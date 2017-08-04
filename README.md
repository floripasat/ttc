# FloripaSat - TTC
## Telemetry, Tracking and Command (TT&amp;C) module of the FloripaSat project

This module is divided in two submodules:
* Beacon
* Telemetry

The software of the telemetry submodule is under development in the OBDH module, and for now is not described in this page.

## Hardware

* [MSP430F6659IPZR](http://www.ti.com/product/MSP430F6659), for the beacon.
* [RF4463F30](http://www.nicerf.com/product_153_140.html), for both beacon and transceiver.

## Software

* All the firmware is written in C
* All the software was developed in the [Code Composer Studio IDE](http://www.ti.com/tool/ccstudio) (Version 7.2.0).
* The Si4463 basic configuration was made in the [WDS](https://www.silabs.com/products/development-tools/software/wireless-development-suite) (Version 3.2.11)

## RF Signal Data

### Beacon

* Frequency: 145,9 MHz
* Modulation: GFSK
* Baudrate: 1,2 ksps
* Deviation: 2,5 kHz
* Power: 30 dBm (1000 mW)

### Transceiver

* Frequency: 437,9 MHz
* Modulation: GFSK
* Baudrate: 2,4 ksps
* Deviation: 5 kHz
* Power: 30 dBm (1000 mW)

## Packets Protocols

The packets are transmitted in two protocols:

* [NGHam](https://github.com/skagmo/ngham) (Telemetry and beacon)
* [AX.25](http://www.ax25.net/) (Beacon)

### Packet Header Data

* Preamble: 4 bytes (0xAA)
* Sync. Word: 4 bytes (0x5D, 0xE6, 0x2A, 0x7E)

## References

All source code of the project can be found in [GitHub](https://github.com/floripasat).

The official website of the project is [www.floripasat.ufsc.br](http://www.floripasat.ufsc.br/).
