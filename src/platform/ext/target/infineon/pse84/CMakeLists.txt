#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.27.7)

# Set CMP0118: The GENERATED source file property is now visible in all
# directories for all targets created in this file
cmake_policy(SET CMP0118 NEW)

# Library with shared read-only data, only SPM can write it
set(IFX_SHARED_RO_DATA_TARGET "ifx_tfm_sp_meta")
add_library(${IFX_SHARED_RO_DATA_TARGET} STATIC)

add_subdirectory_ext(${IFX_COMMON_SOURCE_DIR}/libs libs)

add_subdirectory_ext(${IFX_COMMON_SOURCE_DIR}/board board)

############################# Platform region defs #############################

target_include_directories(platform_region_defs
    INTERFACE
        shared/partition
)

############################## TF-M configuration ##############################

target_compile_definitions(tfm_config
    INTERFACE
        IFX_BSP_CONFIG_HEADER_FILE="${IFX_CONFIG_BSP_PATH}/config_bsp.h"
        $<$<BOOL:${IFX_CM33_NS_PRESENT}>:IFX_CM33_NS_PRESENT=1>
        $<$<BOOL:${IFX_CM55_NS_PRESENT}>:IFX_CM55_NS_PRESENT=1>
)

target_include_directories(tfm_config
    INTERFACE
        $<BUILD_INTERFACE:${IFX_FAMILY_SOURCE_DIR}/config>
        $<BUILD_INTERFACE:${IFX_PLATFORM_SOURCE_DIR}>
)

############################### Platform Secure ###############################

add_subdirectory_ext(spe)
add_subdirectory_ext(${IFX_EPC})

################################ Code coverage #################################

# Code coverage flags must be added after all platform files have been added
# to the build

if(TEST_NS_IFX_CODE_COVERAGE)
    include(${IFX_COMMON_SOURCE_DIR}/cmake/code_coverage.cmake)

    set(IFX_COVERAGE_IGNORE_LIST "")

    # Exclude startup as coverage does not work for it (it is executed prior to
    # copying .data section)
    list(APPEND IFX_COVERAGE_IGNORE_LIST "startup_pse84")

    # Exclude test (we measure coverage of the secure code, not the code that tests it)
    list(APPEND IFX_COVERAGE_IGNORE_LIST "platform/ext/target/infineon/pse84/tests")

    ifx_add_code_coverage(
        EXCLUDE ${IFX_COVERAGE_IGNORE_LIST}
        FILTER_FILE "${IFX_PLATFORM_SOURCE_DIR}/deploy/tfm_files_to_deploy.txt"
    )
endif()

################################################################################

include(./install.cmake)
