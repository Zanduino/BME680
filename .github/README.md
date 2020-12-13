[![License: GPL v3](https://zanduino.github.io/Badges/GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build](https://github.com/Zanduino/BME680/workflows/Build/badge.svg)](https://github.com/Zanduino/BME680/actions?query=workflow%3ABuild) [![Format](https://github.com/Zanduino/BME680/workflows/Format/badge.svg)](https://github.com/Zanduino/BME680/actions?query=workflow%3AFormat) [![Wiki](https://zanduino.github.io/Badges/Documentation-Badge.svg)](https://github.com/Zanduino/BME680/wiki) [![Doxygen](https://github.com/Zanduino/BME680/workflows/Doxygen/badge.svg)](https://Zanduino.github.io/BME680/html/index.html) [![arduino-library-badge](https://www.ardu-badge.com/badge/BME680.svg?)](https://www.ardu-badge.com/BME680)
# BME680 library <img src="https://github.com/Zanduino/BME680/blob/master/Images/bosch-bme680.jpg" alt="Bosch BME680" align="right" height="60px"><br>

*Arduino* library for using the [Bosch BME680](https://www.bosch-sensortec.com/bst/products/all_products/bme680) sensor which senses temperature, humidity and pressure. The BME680 is a tiny package and no hobbyist is going to be breadboarding this sensor directly, so one will be part of a breakout board. Here are some breakout board examples:

| Supplier  | Image |  Instructions | Comments |
| --------- | ----- |------------ | -------- |
| [Sparkfun](https://www.sparkfun.com/products/16466) | <img src="https://cdn.sparkfun.com//assets/parts/1/5/3/2/9/16466-SparkFun_Environmental_Sensor_Breakout_-_BME680__Qwiic_-01a.jpg" width="150px" /> | [Guide](https://learn.sparkfun.com/tutorials/sparkfun-environmental-sensor-breakout---bme680-qwiic-hookup-guide) | The Sparkfun board has pinouts for both SPI and I2C, one interface along one edge and other on the opposing side. The board is designed as part of the SparkFun [Qwiic Connect System](https://www.sparkfun.com/qwiic) that uses standardized plugs to daisy-chain I2C breakout boards together quickly. This package is designed to work with 3V systems and has no on-board 5V level shifting. |
| [Bluedot](https://www.bluedot.space/sensor-boards/bme680/) | <img src="https://github.com/Zanduino/BME680/blob/master/Images/BlueDotBME680.jpg" width="150" /> | [Guide](https://www.bluedot.space/sensor-boards/bme680/) | This was my initial BME680 breakout board, purchased locally in Germany where it is manufactured. It has on-board level shifting and is therefore able to run on both 3V and 5V systems. There are fewer pinouts on this board than on the Sparkfun one, several of the pins have different uses depending upon whether I2C or SPI is selected |
| [Adafruit](https://www.adafruit.com/product/3660) | <img src="https://cdn-shop.adafruit.com/970x728/3660-07.jpg" width="150" /> | [Guide](https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas/) | This breakout board is the most compact of the three, with the pinouts having different uses depending upon which communications method (I2C or SPI) is used. As with the Bluedot breakout board above, the Adafruit breakout board has integrated level-shifting and can therefore be run on both 3V and 5V systems.  |

A complete description of the BME680 sensor can be found in the [BME680 Datasheet](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001-00.pdf).

## Documentation and Examples
The detailed library description including installation instructions and descriptions of all functions plus further details is available on the [BME680 Wiki pages](https://github.com/Zanduino/BME680/wiki). In addition, the code itself contains Doxygen style comments, which is used to generate comprehensive program and library documentation; this can be found at [BME680 Doxygen documentation](https://Zanduino.github.io/BME680/html/index.html).

## Sensing
The BME680 allows the user to individually adjust the sampling rates for each sensor component (temperature, humidity, pressure and gas) as well as to set the inter-measurement delay time and the [IIR](https://en.wikipedia.org/wiki/Infinite_impulse_response) filter sampling coeffient. Combining these settings allows very fine-grained control of measurement times and measurement accuracy as well as energy management.

### Temperature sensing
The BME280 measures temperatures between -40°C and 85°C with a full accuracy range between 0°C and +65°C. The accuracy is ±0.5°c with a full resolution of 0.01°C. Since the temperature sensor in the small package is tied to the PCB board on which it is soldered and also has some self-heating as well the temperature reading will usually be above ambient to some degree.

### Humidity sensing
Non-condensing humidity between 0% and 100% is measured with an accuracy of ±3% at a maximum resolution of 0.008%.

### Pressure sensing
The pressure sensor works in temperatures between -40°C and +85°C although the zone of full accuracy only goes from 0°C to +65°C. Pressure is measured between 300hPa and 1100hPa with an accuracy of ±1.0 hPa and a resolution of 0.2Pa

### Environment gas sensing
The gas sensor works by heating a small surface internally and measuring the resistance of the gas layer. This indicates the amount of volatile components in the air and can be used as an indirect means of measuring of air quality. The actual computation of "indoor air quality" is more complex than measuring the resistance, as the relative humidity, temperature, and element heating all play a role in determining that value. These calculations not only require the use of floating-point math, but are quite complex and memory-intensive and are not included in the library. If IAQ measurements are required, then the best solution is to use Bosch's [BSEC](https://www.bosch-sensortec.com/software-tools/software/bsec/) software  - but keep in mind that it won't run on most smaller Arduino platforms.

[![Zanshin Logo](https://zanduino.github.io/Images/zanshinkanjitiny.gif) <img src="https://zanduino.github.io/Images/zanshintext.gif" width="75"/>](https://zanduino.github.io)
