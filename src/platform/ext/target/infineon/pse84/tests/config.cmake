#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(TEST_NS_IFX_CRYPTO_BENCHMARK OR TEST_NS_IFX_EPC_VALIDATION)
    set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH "${IFX_PLATFORM_SOURCE_DIR}/tests/mbedtls_target_config_pse84.h"    CACHE PATH      "Config to append to standard Mbed Crypto config, used by platforms to configure feature support")
endif()
