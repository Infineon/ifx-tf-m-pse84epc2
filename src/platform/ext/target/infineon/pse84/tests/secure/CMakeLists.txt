#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

####################### Secure ################################################

if (S_BUILD)
    target_sources(tfm_test_suite_extra_s
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/s_test.c
    )
endif()
