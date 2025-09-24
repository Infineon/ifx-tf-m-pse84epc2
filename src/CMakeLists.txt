#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
# Copyright (c) 2022-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(version)
include(remote_library)
include(utils)

if(EXISTS ${CMAKE_SOURCE_DIR}/localrepos.cmake)
    message(WARNING "Building using local repositories from \"" ${CMAKE_SOURCE_DIR}/localrepos.cmake "\"")
    include(${CMAKE_SOURCE_DIR}/localrepos.cmake)
endif()

############################ CONFIGURATION #####################################
include(config/pre_config.cmake)

if(USE_KCONFIG_TOOL)
    include(${CMAKE_SOURCE_DIR}/config/kconfig.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/config/set_config.cmake)
endif()

include(config/post_config.cmake)

############################### Compiler configuration #########################

include(${TFM_TOOLCHAIN_FILE})

set(CMAKE_C_COMPILER_FORCED true)
set(CMAKE_CXX_COMPILER_FORCED true)

project("Trusted Firmware M" VERSION ${TFM_VERSION} LANGUAGES C CXX ASM)
tfm_toolchain_reload_compiler()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    # Limit path length to avoid problems during build on Windows
    set(CMAKE_OBJECT_PATH_MAX 240)

    string(LENGTH "${CMAKE_BINARY_DIR}" CMAKE_BINARY_DIR_LEN)
    if(CMAKE_BINARY_DIR_LEN GREATER 90)
        message(FATAL_ERROR "CMAKE_BINARY_DIR path length (${CMAKE_BINARY_DIR_LEN}) exceeds 90 characters:\n"
            "   \"${CMAKE_BINARY_DIR}\"\n"
            " Please use a shorter build root path!!!")
    endif()
endif()

include(lib/static_checks/static_checks.cmake)
add_subdirectory_ext(lib/ext)
add_subdirectory_ext(lib/fih)
add_subdirectory_ext(tools)
add_subdirectory_ext(secure_fw)

add_subdirectory_ext(interface)
if(BL2)
    add_subdirectory_ext(bl2)
endif()

if(BL1 AND PLATFORM_DEFAULT_BL1)
    add_subdirectory_ext(bl1/bl1_2)
    add_subdirectory_ext(bl1/bl1_1)
endif()

add_subdirectory_ext(platform)

if(CRYPTO_HW_ACCELERATOR)
    add_subdirectory_ext(platform/ext/accelerator)
endif()

if(IS_DIRECTORY ${CONFIG_TFM_TEST_DIR})
    add_subdirectory_ext(${CONFIG_TFM_TEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/tf-m-tests)
endif()

include(platform/ext/target/${TFM_PLATFORM}/post.cmake OPTIONAL)

############################ Config Check ######################################

include(${CMAKE_SOURCE_DIR}/config/check_config.cmake)

################################################################################

include(cmake/install.cmake)
