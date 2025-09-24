/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "cy_pdl.h"
#ifdef IFX_MTB_SRF
#include "cybsp.h"
#include "mtb_srf.h"
#endif /* IFX_MTB_SRF */
#if defined(IFX_MTB_SRF)
#include "ffm/original_vector_api.h"
#endif /* defined(IFX_MTB_SRF) */
#include "ifx_platform_api.h"
#include "ifx_platform_private.h"
#include "platform_svc_api.h"
#include "psa/service.h"
#include "tfm_platform_system.h"


void tfm_platform_hal_system_reset(void)
{
    /* Platform partition may be unprivileged, which means that it does not
     * have access to reset control register, thus SVC call is used. */
    ifx_call_platform_system_reset();
}

#ifdef IFX_MTB_SRF
/* Overwrites default implementation of mtb_srf_memory_validate */
cy_rslt_t mtb_srf_memory_validate(volatile mtb_srf_invec_ns_t* inVec_ns,
                                  uint8_t inVec_cnt_ns,
                                  volatile mtb_srf_outvec_ns_t* outVec_ns,
                                  uint8_t outVec_cnt_ns,
                                  mtb_srf_invec_ns_t* inVec,
                                  mtb_srf_outvec_ns_t* outVec)
{
    /* In TFM case access rights validation is done during psa_call */

    /* Copy in/out vectors to SRF provided buffers */
    for (uint32_t i = 0UL; i < inVec_cnt_ns; ++i) {
        inVec[i].base = inVec_ns[i].base;
        inVec[i].len = inVec_ns[i].len;
    }

    for (uint32_t i = 0UL; i < outVec_cnt_ns; ++i) {
        outVec[i].base = outVec_ns[i].base;
        outVec[i].len = outVec_ns[i].len;
    }

    return CY_RSLT_SUCCESS;
}
#endif /* IFX_MTB_SRF */

psa_status_t ifx_mtb_srf_handler(const psa_msg_t *msg)
{
#ifdef IFX_MTB_SRF
    psa_status_t psa_ret = PSA_ERROR_GENERIC_ERROR;
    tfm_original_iovec_t io_vec;
    memset(&io_vec, 0, sizeof(io_vec));

    psa_ret = original_iovec(msg->handle, &io_vec);
    if (psa_ret != PSA_SUCCESS) {
        return psa_ret;
    }

    return mtb_srf_request_execute(&cybsp_srf_context,
                                   io_vec.invec, io_vec.invec_count,
                                   io_vec.outvec, io_vec.outvec_count);
#else /* IFX_MTB_SRF */
    return PSA_ERROR_NOT_SUPPORTED;
#endif /* IFX_MTB_SRF */
}

psa_status_t tfm_platform_hal_additional_services(const psa_msg_t *msg)
{
    if (msg->type == IFX_PLATFORM_API_ID_MTB_SRF) {
        return ifx_mtb_srf_handler(msg);
    } else {
        return PSA_ERROR_NOT_SUPPORTED;
    }
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
    enum tfm_platform_err_t status = TFM_PLATFORM_ERR_NOT_SUPPORTED;
    switch (request) {
        case IFX_PLATFORM_IOCTL_LOG_MSG: {
            status = (enum tfm_platform_err_t)ifx_call_platform_uart_log(
                    (const uint8_t *)in_vec[0].base, in_vec[0].len);
            break;
        }

        default: {
#if IFX_CUSTOM_PLATFORM_HAL_IOCTL == 1
            if ((request >= IFX_PLATFORM_IOCTL_APP_MIN) &&
                (request <= IFX_PLATFORM_IOCTL_APP_MIN)) {
                /* Handle application platform-specific IOCTL */
                status = ifx_platform_hal_ioctl(request, in_vec, out_vec);
                break;
            }
#endif /* IFX_CUSTOM_PLATFORM_HAL_IOCTL == 1 */

            status = TFM_PLATFORM_ERR_NOT_SUPPORTED;
            break;
        }
    }

    return status;
}
