/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 * Copyright (c) 2021-2023 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "async.h"
#include "config_tfm.h"
#include "ns_agent_mailbox_defs.h"
#include "psa/service.h"
#include "psa_manifest/ns_agent_mailbox.h"
#include "tfm_hal_mailbox.h"
#include "tfm_hal_multi_core.h"
#include "tfm_hal_platform.h"
#include "tfm_multi_core.h"
#include "tfm_rpc.h"
#include "tfm_sp_log.h"

/* Internal states */
enum mailbox_state {
    START,
    NS_CORE_RUNNING,
    LINK_ESTABLISHED,
};

static void boot_ns_core(void)
{
    /* Boot up non-secure core */
    LOG_DBGFMT("Enabling non-secure core...\r\n");

    tfm_hal_boot_ns_cpu(tfm_hal_get_ns_VTOR_off_core());
    tfm_hal_wait_for_ns_cpu_ready();
}

static void state_transition(enum mailbox_state *state, const psa_msg_t *msg)
{
    switch (*state) {
    case START:
        if (msg->type == BOOT_NS_CORE) {
            boot_ns_core();

            if (tfm_inter_core_comm_init()) {
                LOG_ERRFMT("Inter-core communication init failed\r\n");
                psa_panic();
            }

            *state = NS_CORE_RUNNING;
        } else {
            psa_panic();
        }
        break;

    case NS_CORE_RUNNING:
        if (msg->type == ENABLE_MAILBOX) {
            if (tfm_inter_core_comm_enable()) {
                LOG_ERRFMT("Inter-core communication enable failed\r\n");
                psa_panic();
            }
            MAILBOX_ENABLE_INTERRUPTS();

            *state = LINK_ESTABLISHED;
        } else if (msg->type == NS_CORE_SHUTDOWN) {
            if (tfm_inter_core_comm_deinit()) {
                LOG_ERRFMT("Inter-core communication deinit failed\r\n");
                psa_panic();
            }

            *state = START;
        } else {
            psa_panic();
        }
        break;

    case LINK_ESTABLISHED:
        if (msg->type == DISABLE_MAILBOX) {
            MAILBOX_DISABLE_INTERRUPTS();

            if (tfm_inter_core_comm_disable()) {
                LOG_ERRFMT("Inter-core communication disable failed\r\n");
                psa_panic();
            }

            *state = NS_CORE_RUNNING;
        } else {
            psa_panic();
        }
        break;
    }
}

void ns_agent_mailbox_entry(void)
{
    enum mailbox_state state = START;
    psa_signal_t signals = 0;

#if CONFIG_TFM_AUTO_BOOT_NS_CORE
    boot_ns_core();

    if (tfm_inter_core_comm_init()) {
        LOG_ERRFMT("Inter-core communication init failed\r\n");
        psa_panic();
    }

    if (tfm_inter_core_comm_enable()) {
        LOG_ERRFMT("Inter-core communication enable failed\r\n");
        psa_panic();
    }

    MAILBOX_ENABLE_INTERRUPTS();

    state = LINK_ESTABLISHED;
#endif

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        while (MAILBOX_SIGNAL_IS_ACTIVE(signals)) {
            psa_eoi(MAILBOX_SIGNAL_GET_ACTIVE(signals));
            /* Discard late-arriving signals */
            if (state == LINK_ESTABLISHED) {
                tfm_rpc_client_call_handler();
            }
            signals &= ~MAILBOX_SIGNAL_GET_ACTIVE(signals);
        }
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
        if (signals & ASYNC_MSG_REPLY) {
            if (state == LINK_ESTABLISHED) {
                tfm_rpc_client_call_reply();
            } else {
                /* If the mailbox is not active, this is a response to
                 * a request that we've already rejected, so clean up
                 * but don't actually try to send the response
                 */
                tfm_rpc_client_drop_reply();
            }
            signals &= ~ASYNC_MSG_REPLY;
        }
#endif
        if (signals & TFM_NS_AGENT_MAILBOX_SERVICE_SIGNAL) {
            psa_msg_t msg;
            psa_status_t status = psa_get(TFM_NS_AGENT_MAILBOX_SERVICE_SIGNAL, &msg);
            if (status != PSA_SUCCESS) {
                continue;
            }

            state_transition(&state, &msg);

            psa_reply(msg.handle, PSA_SUCCESS);

            signals &= ~TFM_NS_AGENT_MAILBOX_SERVICE_SIGNAL;
        }

        if (signals != 0) {
            /* Wrong signal asserted */
            psa_panic();
        }
    }
}
