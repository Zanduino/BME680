# Example programs for the BME680 library<br>[![Build Status](https://travis-ci.org/SV-Zanshin/BME680.svg?branch=master)](https://travis-ci.org/SV-Zanshin/BME680) [![DOI](https://zenodo.org/badge/139349456.svg)](https://zenodo.org/badge/latestdoi/139349456) [![arduino-library-badge](https://www.ardu-badge.com/badge/BME680.svg?)](https://www.ardu-badge.com/BME680)

Various example programs illustrating the use of the *Arduino* BME680 library are included in this directory and sub-directories.

| Example Name      | Wiki Link                                                                                | Comments |
| ----------------- | ---------------------------------------------------------------------------------------- | -------- |
| I2C Demo          | [I2CDemo.ino](https://github.com/SV-Zanshin/BME680/wiki/I2CDemo.ino)                     | Program to set up and continuously read from a BME680 connected via the I2C pins (SDA and SCL) |
| SPI Demo          | [SPIDemo.ino](https://github.com/SV-Zanshin/BME680/wiki/SPIDemo.ino)                     | Program to set up and continuously read from a BME680 connected using the default hardware SPI pins (SCL,MOSI, MISO and SS) |
| software SPI Demo | [SoftSPIDemo.ino](https://github.com/SV-Zanshin/BME680/wiki/SoftSPIDemo.ino)             | Identical to the I2C Demo program, but using SPI connectivity emulated in software |
| Two I2C BME680s   | [TwoDevicesI2CDemo.ino](https://github.com/SV-Zanshin/BME680/wiki/TwoDevicesI2CDemo.ino) | Example to illustrate attaching 2 BME680 devices with different addresses using I2C |
| SD-Card Logging   | [SDLoggerI2CDemo.ino](https://github.com/SV-Zanshin/BME680/wiki/SDLoggerI2CDemo.ino)     | Example sketch to log data to a SD-Card at variable rates which speed up when the measurements change rapidly and slows down when there is little change. |

![Zanshin Logo](https://www.sv-zanshin.com/r/images/site/gif/zanshinkanjitiny.gif) <img src="https://www.sv-zanshin.com/r/images/site/gif/zanshintext.gif" width="75"/>
