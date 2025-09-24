#-------------------------------------------------------------------------------
# Copyright (c) 2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

##### Define PSE84 non-secure test specific cmake configurations here #######

if (IFX_CORE STREQUAL ${IFX_CM55})
    # Following tests are not supported on CM55
    set(TEST_NS_FPU             OFF         CACHE BOOL      "Whether to build NS regression FPU tests" FORCE)
endif()
