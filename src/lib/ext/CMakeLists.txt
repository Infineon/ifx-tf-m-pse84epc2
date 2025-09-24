#-------------------------------------------------------------------------------
# Copyright (c) 2020-2024, Arm Limited. All rights reserved.
# Copyright (c) 2020-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/lib/ext CACHE STRING "" FORCE)

add_subdirectory_ext(qcbor)
add_subdirectory_ext(t_cose)
add_subdirectory_ext(mbedcrypto)
add_subdirectory_ext(cmsis)
if(BL2 OR (TFM_PARTITION_FIRMWARE_UPDATE AND (${TFM_FWU_BOOTLOADER_LIB} STREQUAL "mcuboot")))
    add_subdirectory_ext(mcuboot)
endif()
if (${PLATFORM_PSA_ADAC_SECURE_DEBUG})
    add_subdirectory_ext(psa-adac)
endif()
if(DEFINED ETHOSU_ARCH)
    add_subdirectory_ext(ethos_u_core_driver)
endif()

add_subdirectory_ext(tf-m-extras)
