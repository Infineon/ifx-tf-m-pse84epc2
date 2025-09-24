/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef IFX_PLATFORM_TESTS_SPE_CONFIG_H
#define IFX_PLATFORM_TESTS_SPE_CONFIG_H

#define IFX_PC_TZ_NSPE_ID               0x02UL    /* CM33 NSPE Application */

/* ITCM address used to validate access for mailbox NS client */
#define IFX_MAILBOX_ITCM_ADDRESS_1      0x00000010u

/* ITCM address mapped to CM33 address space used to validate access for mailbox NS client */
#define IFX_MAILBOX_ITCM_ADDRESS_2      0x48000010u

/* DTCM address used to validate access for mailbox NS client */
#define IFX_MAILBOX_DTCM_ADDRESS_1      0x20000010u

/* DTCM address mapped to CM33 address space used to validate access for mailbox NS client */
#define IFX_MAILBOX_DTCM_ADDRESS_2      0x48040010u

#endif /* IFX_PLATFORM_TESTS_SPE_CONFIG_H */
