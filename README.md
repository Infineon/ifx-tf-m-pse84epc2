# Infineon Customized Trusted Firmware-M for Secure Projects

## Overview

Trusted Firmware-M (TF-M) provides a Secure Processing Environment (SPE) for Arm Cortex-M platforms and aligns with PSA Certified guidelines. TF-M enables chips and devices to achieve PSA Certification.

This repository contains TF-M for secure projects. For non-secure projects (NSPE), use the [ifx-tf-m-ns](https://github.com/Infineon/ifx-tf-m-ns) library to integrate TF-M services.

## Licensing

This software is licensed under a combination of the Apache License 2.0 and the BSD 3-Clause License.
Please review the license files for individual modules:
- [t_cose LICENSE](src/lib/ext/t_cose/LICENSE)
- [qcbor README](src/lib/ext/qcbor/README.md)

## Using TF-M in the ModusToolbox™ Tools Package

### Building a Secure Image

1. Add the `ifx-tf-m` library to your secure project.
2. Build your secure project.

> **Note:** Files such as `main.c`, linker scripts, and startup code provided by the ModusToolbox™ secure project will be replaced by those from TF-M sources.

### Using TF-M in Non-Secure Applications

1. For platforms with multiple non-secure cores, enable and initialize NSPE(s) according to your platform's instructions.
2. Add the [ifx-tf-m-ns](https://github.com/Infineon/ifx-tf-m-ns) library to your non-secure project.
3. The code for binding a non-secure project with TF-M is generated during the secure build and placed in `TFM_INSTALL_PATH`.

### Edge Protect Solution Personality

Edge Protect Solution helps you configure memory, peripherals, and protection domains for TF-M.

TF-M library provides secure services, which can be configured in the Edge Protect Configurator (EPC).
Within EPC, you can assign memory regions and peripherals to these services via protection domains. Each TF-M service has own protection domain.
SPM (Secure Partition Manager) is one of the core TF-M services, and it is managed by the M33S protection domain. Other secure services are also mapped to their respective protection domains, allowing you to flexibly attach memory and peripherals to each service as needed.

**Edge Protect Configurator automatically creates protection domains and applies default configuration for memory regions and peripherals, assigning them to the relevant domains.**
Manual configuration and assignment of memory regions and peripherals to protection domains is only possible via the Device Configurator.

- **Edge Protect Configurator**
  The Edge Protect Configurator is a tool that provides a GUI for the TF-M library configuration.
  To launch the Configurator, press the "Launch Edge Protect Configurator" button on the Parameters pane.
  The Configurator works in cooperation with the Edge Protect Solution personality to configure protections, memory, and peripheral resources for the TF-M library.
  It allows you to link memory regions and peripherals to specific TF-M services through their protection domains (service domains), using automatic default settings.
  For advanced manual configuration or custom assignments, use the Device Configurator.
  Refer to the Edge Protect Configurator user guide for more details on the configuration process.

- **Protection Domains**
  Protection domains are created automatically when TF-M services are enabled.
  Each TF-M service operates within its own protection domain, which isolates its memory and peripheral resources from others.
  The SPM service specifically is managed by the M33S protection domain.
  Review or modify protection domain parameters using the "Show Expert-Level Protection Domain Parameters" checkbox.

- **Memory Configuration**
  Required memory regions are created when enabling certain TF-M services.
  EPC assigns these regions to protection domains by default.
  For manual configuration or reassignment, use the Device Configurator's Memory tab.

- **Peripheral Configuration**
  Some TF-M services require specific peripheral protection.
  EPC assigns these peripherals to the appropriate protection domain by default (for example, M33S for SPM).
  For manual configuration or reassignment, use the System tab in the Device Configurator.

### ModusToolbox™ Makefile Options

Configure your build using the following variables in your Makefile:

- `TFM_GIT_URL`: Optional URL for a custom TF-M source repository.
    - `TFM_GIT_REF`: Commit, branch, or tag reference.
- `TFM_SRC_DIR`: Path to TF-M sources.
- Library location variables:
    - `IFX_CORE_LIB_PATH`
    - `IFX_DEVICE_DB_LIB_PATH`
    - `IFX_MBEDTLS_ACCELERATION_LIB_PATH`
    - `IFX_PDL_LIB_PATH`
    - `IFX_SE_RT_SERVICES_UTILS_PATH`
    - `MBEDCRYPTO_PATH`
- `TFM_BUILD_DIR`: Location of the build directory.
- `TFM_COMPILE_COMMANDS_PATH`: Path for `compile_commands.json`.
- `TFM_DEBUG_SYMBOLS`: Enable debug information (does not affect optimization).
- `TFM_CMAKE_BUILD_TYPE`: Override CMake build type.
- `CONFIG`: Sets `CMAKE_BUILD_TYPE` if `TFM_CMAKE_BUILD_TYPE` is not specified. Possible values:
    - `Debug`: MinSizeRel with debug information.
    - `Release`: MinSizeRel without debug information.
- `TFM_TOOLS_CMAKE`: Path to the CMake executable.
    - `TFM_TOOLS_CMAKE_URL`: Custom URL for CMake download.
- `TFM_INSTALL_PATH`: Optional install path for the non-secure interface.
- `TFM_CONFIGURE_EXT_OPTIONS`: Additional CMake options.

#### Example Makefile Fragment

```makefile
TFM_GIT_URL=https://github.com/your-org/trusted-firmware-m.git
TFM_GIT_REF=main
TFM_BUILD_DIR=./build
TFM_DEBUG_SYMBOLS=ON
```

### Using Additional Libraries in the TF‑M Secure Image

When adding a custom partition, you may need to use additional libraries in the
TF-M secure image.

To do so:
1. Add the library to the secure project via the Library Manager
    (e.g. the `ethernet-phy-driver` library).
2. At the end (last line) of the secure project Makefile, specify the path to
   the library folder, e.g.:
   ```makefile
   $(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_MTB_ETHERNET_PHY_DRIVER_LIB_PATH,IFX_MTB_ETHERNET_PHY_DRIVER_LIB_PATH,SEARCH_ethernet-phy-driver))
   ```
3. Add the library files to the build. This can be done by either manually
   specifying the required files and include directories (refer to the standard
   CMake build system commands), or by using the helper `ifx_mtb_autodiscovery`
   function (refer to `platform/ext/target/infineon/common/cmake/mtb.cmake` for
   the function documentation), e.g. in the partition CMake file add:
   ```cmake
   add_library(ifx_ethernet_phy_driver STATIC EXCLUDE_FROM_ALL)

   target_compile_options(ifx_ethernet_phy_driver
       PRIVATE
           ${COMPILER_CMSE_FLAG}
   )

   include(${IFX_COMMON_SOURCE_DIR}/cmake/mtb.cmake)

   ifx_mtb_autodiscovery(
       PATH ${IFX_MTB_ETHERNET_PHY_DRIVER_LIB_PATH}
       TARGET ifx_ethernet_phy_driver
       COMPONENTS ""
   )

   target_link_libraries(ifx_ethernet_phy_driver
       PRIVATE
           ifx_pdl_inc_s
   )

   target_link_libraries(tfm_app_rot_partition_custom_partition
       PRIVATE
           ifx_ethernet_phy_driver
   )
   ```

### SPM Logging

TF-M supports logging for the Secure Partition Manager (SPM).
To enable SPM logging in your ModusToolbox™ project:

1. Configure SCBx to UART mode in Device Configurator and name it `IFX_TFM_SPM_UART`.
2. Assign the SCBx PPC region to the M33S (SPM) domain.
3. Set the `IFX_UART_ENABLED` CMake option to `ON`.
4. Set `TFM_SPM_LOG_LEVEL` for SPM logs:
    - `TFM_SPM_LOG_LEVEL_DEBUG`: All logs.
    - `TFM_SPM_LOG_LEVEL_INFO`: All except debug logs.
    - `TFM_SPM_LOG_LEVEL_ERROR`: Only errors.
    - `TFM_SPM_LOG_LEVEL_SILENCE`: No logs.
5. Set `TFM_PARTITION_LOG_LEVEL` for secure partition logs:
    - `TFM_PARTITION_LOG_LEVEL_DEBUG`: All logs.
    - `TFM_PARTITION_LOG_LEVEL_INFO`: All except debug logs.
    - `TFM_PARTITION_LOG_LEVEL_ERROR`: Only errors.
    - `TFM_PARTITION_LOG_LEVEL_SILENCE`: No logs.

> **Note:** Only the SPM domain (M33S) can access `IFX_TFM_SPM_UART` when logging is enabled.

To write logs from a secure partition or a non-secure project, use:
```c
ifx_platform_log_msg("Your log message");
```

## TF-M Configuration

### CMake Options

Add extra CMake options via `TFM_CONFIGURE_EXT_OPTIONS`:

- `TFM_PROFILE`: TF-M profile.
- `TFM_ISOLATION_LEVEL`: TF-M isolation level.
- `IFX_MBEDTLS_ACCELERATION_ENABLED`: Enable hardware crypto acceleration.
    - Requires the `cy-mbedtls-acceleration` library.
- `IFX_MBEDTLS_CONFIG_PATH`: Optional path to the mbedtls config header.
- `IFX_MTB_SRF`: Enable MTB SRF support.
- `IFX_PROJECT_CONFIG_PATH`: Optional path to the project config header.
- Other options: See [TF-M documentation](https://tf-m-user-guide.trustedfirmware.org/index.html).

## Static Code Checkers

TF-M code is checked against MISRA-C and CERT-C rules.

**MISRA-C rules ignored:**
1.2; 1.5; 2.1; 2.3–2.5; 2.7; 3.1; 5.1; 5.6–5.8; 8.3; 8.4–8.9; 8.13; 8.15; 10.3; 10.5–10.8; 11.1; 11.4; 11.5; 11.9; 12.1; 13.3–13.5; 14.3; 14.4; 15.5; 16.1; 16.3; 16.6; 17.7; 20.7; 20.9; 21.1; 21.2; 21.6; 21.15; 21.16.

**MISRA-C directives ignored:** 4.4; 4.10.

**CERT-C rules ignored:** EXP32-C; EXP33-C; EXP34-C; INT30-C; INT31-C; INT33-C; STR30-C; STR31-C.

Other rules may be suppressed for specific lines; see the TF-M source code for details.

## More Information

- [Infineon](https://www.infineon.com)
- [Infineon GitHub](https://github.com/Infineon)
- [Trusted Firmware](https://www.trustedfirmware.org)
- [TF-M Project](https://www.trustedfirmware.org/projects/tf-m)
- [PSA API](https://github.com/ARM-software/psa-arch-tests/tree/master/api-specs)
- [ModusToolbox Documentation](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)
- [TF-M User Guide](https://tf-m-user-guide.trustedfirmware.org/index.html)

---

© 2023–2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
