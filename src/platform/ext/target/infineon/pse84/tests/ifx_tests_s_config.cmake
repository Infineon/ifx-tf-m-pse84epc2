#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########### Define PSE84 test specific cmake configurations here ############

if(TEST_NS_IFX_ISOLATION_TEST OR (NOT DEFINED TEST_NS_IFX_ISOLATION_TEST AND TEST_NS_IFX))
    # PSE84 tests uses wdt so halt on core panic instead of reboot
    set(CONFIG_TFM_HALT_ON_CORE_PANIC           ON         CACHE BOOL    "On fatal errors in the secure firmware, halt instead of rebooting.")
endif()
