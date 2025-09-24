/*
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "config_tfm.h"
#include "ns_agent_mailbox_defs.h"
#include "ns_agent_mailbox_interface.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"


psa_status_t ns_agent_boot_ns_core(void)
{
    return psa_call(TFM_NS_AGENT_MAILBOX_SERVICE_HANDLE, BOOT_NS_CORE, NULL, 0, NULL, 0);
}

psa_status_t ns_agent_mailbox_enable(void)
{
    return psa_call(TFM_NS_AGENT_MAILBOX_SERVICE_HANDLE, ENABLE_MAILBOX, NULL, 0, NULL, 0);
}

psa_status_t ns_agent_mailbox_disable(void)
{
    return psa_call(TFM_NS_AGENT_MAILBOX_SERVICE_HANDLE, DISABLE_MAILBOX, NULL, 0, NULL, 0);
}

psa_status_t ns_agent_ns_core_shutdown(void)
{
    return psa_call(TFM_NS_AGENT_MAILBOX_SERVICE_HANDLE, NS_CORE_SHUTDOWN, NULL, 0, NULL, 0);
}
