# BME280 library
<img src="https://github.com/SV-Zanshin/BME280/blob/master/Images/sensors_pinout.jpg" width="175" align="right"/> *Arduino* library for using the [Bosch BME280](https://www.bosch-sensortec.com/bst/products/all_products/bme280) sensor which senses temperature, humidity and pressure. Since the package is quite small and difficult to breadboard with, the [Adafruit BME280 Breakout](https://www.adafruit.com/product/2652) board was used for development.

A complete description of the BME280 sensor can be found in the [BME280 Datasheet](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf) while the [Adafruit BME280 tutorial](https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout) describes additional board information.

## Sensing
The BME280 allows the user to individually adjust the sampling rates for each component sensing (temperature, humidity and pressure) as well as to set the inter-measurement delay time and the [IIR](https://en.wikipedia.org/wiki/Infinite_impulse_response) filter sampling coeffient. Combining these settings allows very fine-grained control of measurement times vs. measurement accuracy.

### Temperature sensing
The BME280 measures temperatures between -40°C and 85°C with a full accuracy range between 0°C and +65°C. The accuracy is ±0.5°c with a full resolution of 0.01°C. Since the temperature sensor in the small package is tied to the PCB board on which it is soldered placed and has some self-heating as well the temperature reading will usually be above ambient.
### Humidity sensing
Non-condensing humidity between 0% and 100% is measured with an accuracy of ±3% at a maximum resolution of 0.008%.
### Pressure sensing
The pressure sensor works in temperatures between -40°C and +85°C although the zone of full accuracy only goes from 0°C to +65°C. Pressure is measured between 300hPa and 1100hPa with an accuracy of ±1.0 hPa and a resolution of 0.2Pa

A detailed library description and further details are available at the [GitHub BME280 Wiki](https://github.com/SV-Zanshin/BME280/wiki)

[![DOI](https://www.zenodo.org/badge/98804600.svg)](https://www.zenodo.org/badge/latestdoi/98804600)

![Zanshin Logo](https://www.sv-zanshin.com/r/images/site/gif/zanshinkanjitiny.gif) <img src="https://www.sv-zanshin.com/r/images/site/gif/zanshintext.gif" width="75"/>
