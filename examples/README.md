[![License: GPL v3](https://zanduino.github.io/Badges/GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build](https://github.com/Zanduino/BME680/workflows/Build/badge.svg)](https://github.com/Zanduino/BME680/actions?query=workflow%3ABuild) [![Format](https://github.com/Zanduino/BME680/workflows/Format/badge.svg)](https://github.com/Zanduino/BME680/actions?query=workflow%3AFormat) [![Wiki](https://zanduino.github.io/Badges/Documentation-Badge.svg)](https://github.com/Zanduino/BME680/wiki) [![Doxygen](https://github.com/Zanduino/BME680/workflows/Doxygen/badge.svg)](https://Zanduino.github.io/BME680/html/index.html) [![arduino-library-badge](https://www.ardu-badge.com/badge/BME680_Zanshin.svg?)](https://www.ardu-badge.com/BME680_Zanshin)
# Example programs for the BME680 library<br>

Various example programs illustrating the use of the *Arduino* BME680 library are included in this directory and sub-directories.

| Example Name        | Wiki Link                                                                                | Comments |
| ------------------- | ---------------------------------------------------------------------------------------- | -------- |
| I2C Demo            | [I2CDemo.ino](https://github.com/Zanduino/BME680/wiki/I2CDemo.ino)                     | Program to set up and continuously read from a BME680 connected via the I2C pins (SDA and SCL) |
| SPI Demo            | [SPIDemo.ino](https://github.com/Zanduino/BME680/wiki/SPIDemo.ino)                     | Program to set up and continuously read from a BME680 connected using the default hardware SPI pins (SCL,MOSI, MISO and SS) |
| software SPI Demo   | [SoftSPIDemo.ino](https://github.com/Zanduino/BME680/wiki/SoftSPIDemo.ino)             | Identical to the I2C Demo program, but using SPI connectivity emulated in software |
| Two I2C BME680s     | [TwoDevicesI2CDemo.ino](https://github.com/Zanduino/BME680/wiki/TwoDevicesI2CDemo.ino) | Example to illustrate attaching 2 BME680 devices with different addresses using I2C |
| SD-Card Logging     | [SDLoggerSPIDemo.ino](https://github.com/Zanduino/BME680/wiki/SDLoggerSPIDemo.ino)     | Example sketch using SPI for the BME680 and a SD-Card to log data at variable rates - faster logging when the measurements change rapidly and less frequent measurements when there is little change. |
| ESP32 WiFi demo     | [ESP32FeatherWiFiDemo.ino](https://github.com/Zanduino/BME680/wiki/ESP32FeatherWiFiDemo.ino)     | Example sketch using I2C for the BME680 (and an optional SD-Card on SPI to log data) to connect to WiFi and dynamically display measurement data on a web page. |

[![Zanshin Logo](https://zanduino.github.io/Images/zanshinkanjitiny.gif) <img src="https://zanduino.github.io/Images/zanshintext.gif" width="75"/>](https://zanduino.github.io)
