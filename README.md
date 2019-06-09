# SiLabs Thunderboard Sense Firmware Source Code

The firmware is based on the original SiLabs Thunderboard Sense firmware.
Thunderboard Sense (`SLTB001A`) can be purchased from [Digikey](https://www.digikey.com/product-detail/en/silicon-labs/SLTB001A/336-3738-ND/6056619).

This is the version used in the capacitive power transfer (CPT) project.
It has disabled the power source detection and forces the system to run in low-power mode.
You will need SiLabs' Simplicity Studio to build the project.

A separate version that can be directly built from standard GCC toolchain might be released in the future.
However, the resultant firmware would be different, despite the same function.

Power shall be directly applied to the `GND` and `VMCU` pins in the pin headers (position labeled 1 and 2 on the board, respectively).
Design files of the board can be downloaded from [SiLabs' website](https://www.silabs.com/products/development-tools/thunderboard/thunderboard-sense-kit).

For license, see each individual file.
