#-------------------------------------------------------------------------------
# Copyright (c) 2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TFM_S_REG_TEST OR TFM_NS_REG_TEST OR (DEFINED TEST_PSA_API))
    add_subdirectory_ext(${IFX_COMMON_SOURCE_DIR}/tests)
endif()
