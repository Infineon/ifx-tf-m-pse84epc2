# Infineon Customized Trusted Firmware-M Release Notes

## What's Included?
Trusted Firmware-M (TF-M) implements the Secure Processing Environment (SPE)
for Arm Cortex-M based platforms. This aligns the reference implementation of the platform security architecture
with the PSA Certified guidelines. Thus, TF-M allows relevant chips and devices to become PSA Certified.

## Changelog

### v2.1.300
* Removed support of IFX_PDL_SECURE_SERVICES
* Added support of IFX_MTB_SRF
* Renamed the ifx-trusted-firmware-m-ns library to ifx-tf-m-ns
* Instead of the single ifx-trusted-firmware-m library, dedicated libraries have been created for each platform:
    * ifx-tf-m-pse84epc2 for PSE84 EPC2
    * ifx-tf-m-pse84epc4 for PSE84 EPC4
    * ifx-tf-m-psc3 for PSC3

### v2.1.200
* Added support of PSE84 EPC4
* Added support of IFX_PDL_SECURE_SERVICES
* Added support of the Edge Protect Configurator
* Added support of memory layout configuration via Memory configurator integrated into the Device Configurator
* Deprecated the ifx-src-trusted-firmware-m library. TF-M sources are part of
  the ifx-trusted-firmware-m library.
* Added a new ifx-trusted-firmware-m-ns library to integrate builds of the TF-M non-secure interface into
  a non-secure project.

### v2.1.100
* TF-M 2.1.0
* Added support of PSC3

### v2.0.10
* TF-M 2.0.0 with external memory support for PSE84 EPC2

### v1.8.0
* Initial release for TF-M with support for PSE84 EPC2

## Supported Software and Tools
This version of TF-M was validated for compatibility with the following Software and Tools:

| Software and Tools                                                            | Version |
| :---                                                                          | :----:  |
| CMake                                                                         | 3.27.7  |
| Python Jinja2 module                                                          | Latest  |
| Python PyYAML module                                                          | Latest  |
| Python cbor module                                                            | >=1.0.0 |

## More information
Use the following links for more information, as needed:
* [Cypress Semiconductor Corporation (an Infineon company)](https://www.infineon.com)
* [Cypress Semiconductor Corporation (an Infineon company) GitHub](https://github.com/Infineon)
* [Trusted Firmware website](https://www.trustedfirmware.org)
* [TF-M project](https://www.trustedfirmware.org/projects/tf-m)
* [PSA API](https://github.com/ARM-software/psa-arch-tests/tree/master/api-specs)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)

---
© 2023-2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
