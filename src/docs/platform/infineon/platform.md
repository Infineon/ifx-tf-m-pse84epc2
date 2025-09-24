# PSE84 TF-M specifics
## Memory configuration
TF-M uses the memory regions that are defined in the Device Configurator.
Edge Protect Solution can be used to configure a TF-M compatible memory map.
Refer to the `Edge Protect Solution Personality` section for more details.

## Enabling and initializing NSPEs for PSE84
The PSE84 platform supports a Non-Secure Processing Environment (NSPE) on the
Cortex-M33 core, with an optional NSPE on the Cortex-M55 core.

The CM33 NSPE is always enabled. This is required because the CM33 NSPE acts as
a proxy for the CM55 NSPE PSA and SRF calls, forwarding them to TF-M.

In the CM33 NSPE, the NS interface must be initialized by calling the
`tfm_ns_interface_init` function.

Starting the CM55 NSPE is the responsibility of the CM33 NSPE. On the CM55, the
communication layer is initialized in `cybsp_init`. After the call to `cybsp_init`,
the CM55 NSPE can perform PSA and SRF function calls.

## PSE84 TF-M logging
TF-M logging is performed over the UART interface defined by the `IFX_TFM_SPM_UART`
alias. The SCB peripheral corresponding to `IFX_TFM_SPM_UART` should have its PPC
region, TX and RX pins set to secure in the Device Configurator.

## Default configuration
TF-M is configured by default with the following settings:
| Description                        | Configuration option                   | PSE84 EPC2     | PSE84 EPC4     |
| ---------------------------------- | -------------------------------------- | -------------- | -------------- |
| Profile                            | TFM_PROFILE                            | profile_medium | profile_medium |
| Crypto partition                   | TFM_PARTITION_CRYPTO                   | ON             | ON             |
| Use Crypto accelerator             | IFX_MBEDTLS_ACCELERATION_ENABLED       | ON             | ON             |
| Firmware Update partition          | TFM_PARTITION_FIRMWARE_UPDATE          | OFF            | OFF            |
| Initial Attestation partition      | TFM_PARTITION_INITIAL_ATTESTATION      | ON             | ON             |
| Internal Trusted Storage partition | TFM_PARTITION_INTERNAL_TRUSTED_STORAGE | ON             | ON             |
| Platform partition                 | TFM_PARTITION_PLATFORM                 | ON             | ON             |
| Protected Storage partition        | TFM_PARTITION_PROTECTED_STORAGE        | ON             | ON             |
| Isolation Level                    | TFM_ISOLATION_LEVEL                    | 2              | 3              |
| Fault Injection Hardening          | TFM_FIH_PROFILE                        | OFF            | HIGH           |
| CM33 Non-Secure image              | IFX_CM33_NS_PRESENT                    | ON             | ON             |
| CM55 Non-Secure image              | IFX_CM55_NS_PRESENT                    | ON             | ON             |
| MTB SRF Support                    | IFX_MTB_SRF                            | ON             | ON             |

> **_NOTE:_** The Firmware Update service is not supported for the PSE84 device.

---
© 2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
