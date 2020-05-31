/*! @file ESP32FeatherWiFiDemo.ino

@section ESP32FeatherWiFiDemo_intro_section Description

Example program for using an Arduino ESP32 based system (this sketch was developed and tested on a ESP32 Huzzah32 
Feather board from https://www.adafruit.com/product/3405) along with a BME680 connected via I2C to monitor the
temperature, pressure and humidity and report the values in a dynamic chart on a web page hosted by the ESP32
and connected to a local network.

Prior to compiling the program, the contents of the include file "Authentication.h" need to be updated to reflect 
the local WiFi network to use and the corresponding authentication code.

Once started, the IP-Address is set by the WiFi router and displayed on the serial output of the ESP32, this IP
address should then be entered as the URL in a web browser of a computer attached to the same network and the 
data should be presented there, updated every 5 seconds.

The Bosch BME680 sensor measures temperature, pressure, humidity and air quality and is described at
https://www.bosch-sensortec.com/bst/products/all_products/BME680. The datasheet is available from Bosch at 
https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680_DS001-11.pdf \n\n

The most recent version of the BME680 library is available at https://github.com/SV-Zanshin/BME680 and the
documentation of the library as well as example programs are described in the project's wiki pages located at
https://github.com/SV-Zanshin/BME680/wiki. \n\n

The BME680 is an extremely small physical package that is so tiny as to be impossible to solder at home, hence it
will be used as part of a third-party breakout board. There are several such boards available at this time, for
example \n
Company  | Link
-------  | ----------
Sparkfun | https://www.sparkfun.com/products/14570
BlueDot  | https://www.bluedot.space/sensor-boards/bme680/
Adafruit | https://learn.adafruit.com/adafruit-BME680-humidity-barometric-pressure-temperature-sensor-breakout \n\n

Bosch supplies sample software that runs on various platforms, including the Arduino family; this can be downloaed
at https://github.com/BoschSensortec/BSEC-Arduino-library . This software is part of the Bosch "BSEC" (Bosch
Sensortec Environmental Cluster) framework and somewhat bulky and unwieldy for typical Arduino applications, hence
the choice to make a more compact and rather less abstract library.

The pressure reading needs to be adjusted for altitude to get the adjusted pressure reading. There are numerous
sources on the internet for formulae converting from standard sea-level pressure to altitude, see the data sheet
for the BME180 on page 16 of http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf. Rather than put a
floating-point function in the library which may not be used but which would use space, an example altitude
computation function has been added to this example program to show how it might be done.

@section ESP32FeatherWiFiDemolicense License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section ESP32FeatherWiFiDemoauthor Author

Written by Arnd\@SV-Zanshin

@section ESP32FeatherWiFiDemoversions Changelog

Version | Date       | Developer                     | Comments
------- | ---------- | ----------------------------- | -------------------------------------------
1.0.0b  | 2020-05-30 | https://github.com/SV-Zanshin | Initial coding
*/

#if !defined(ESP32)
  #error This example program is designed specifically for the ESP32 platform and might not work on other platforms
#endif

/*******************************************************************************************************************
** Declare all include files required                                                                             **
*******************************************************************************************************************/
#include <SPI.h>                                     ///< Include the SPI standard library
#include <SD.h>                                      ///< Include the SD Card standard library
#include <WiFi.h>                                    ///< ESP32 WiFi Library
#include <WiFiClient.h>                              ///< WiFi Client library 
#include <WebServer.h>                               ///< WiFi Web Server library
#include "WebPageContents.h"                         ///< Include external HTML and JavaScript definitions
#include "Authentication.h"                          ///< Contains the network SSID and authentication code
#include "Zanshin_BME680.h"                          ///< The BME680 sensor library

/*******************************************************************************************************************
** Declare all program constants                                                                                  **
*******************************************************************************************************************/
const uint32_t  SERIAL_SPEED       =         115200; ///< Set the baud rate for Serial I/O
const uint8_t   POWER_PIN          =            A13; ///< Supply voltage through a divider
const uint8_t   LED_PIN            =             13; ///< This pin is the on-board red LED
const uint8_t   SD_CARD_SPI_CS_PIN =             21; ///< Use Pin 21 (general GPIO) on the ESP32 for chip select
const uint8_t   SD_CARD_SPI_CD_PIN =             A5; ///< Use Pin A5 (general GPIO) on the ESP32 for carrier detect
const char*     FILE_NAME          = "/BME_680.csv"; ///< Filename on SD-Card
const uint32_t  SD_LOG_INTERVAL    =           1000; ///< Milliseconds between measurements to SD-Card
const uint8_t   SD_FLUSH_INTERVAL  =             60; ///< do a "flush" after this number of writes

/*******************************************************************************************************************
** Declare all program macros                                                                                     **
*******************************************************************************************************************/
#define VOLTAGE (analogRead(POWER_PIN)/4095.0*3.3*2*1.1) ///< Macro for floating point voltage
/*******************************************************************************************************************
** Declare all global variables                                                                                   **
*******************************************************************************************************************/
WebServer    server(80);                             ///< Instantiate a web server on port 80
BME680_Class BME680;                                 ///< Create an instance of the BME680 class
File         dataFile;                               ///< Class for a SD-Card file
int32_t      temperature;                            ///< BME680 temperature value
int32_t      humidity;                               ///< BME680 humidity value
int32_t      pressure;                               ///< BME680 pressure value
int32_t      gas;                                    ///< BME680 gas resistance value
int32_t      start_pressure;                         ///< Initial pressure reading
bool         sd_card_present = false;                ///< Switch set when SD-Card detected
String       jsonData;                               ///< JSON data string 
uint16_t     loopCounter = 0;                        ///< Counter for number of write operations since startup
uint32_t     next_log_millis;                        ///< Millis() value for next SD-Card measurement

void setup()
{
  pinMode(SD_CARD_SPI_CS_PIN, OUTPUT);               // Declare the Chip-Select pin for the SD Card as output
  pinMode(SD_CARD_SPI_CD_PIN, INPUT_PULLUP);         // Declare the Carrier detect pin for the SD Card as input
  digitalWrite(SD_CARD_SPI_CS_PIN, HIGH);            // Write a high value to it in order to deselect device
  pinMode(LED_PIN, OUTPUT);                          // make the LED an output pin
  Serial.begin(115200);
  Serial.print(F("Starting ESP32FeatherWiFiDemo example program for BME680\n"));
  /**********************
  ** Initialize BME680 **
  **********************/
  Serial.print(F("- Initializing BME680 sensor\n"));
  while (!BME680.begin(I2C_STANDARD_MODE)) // Start BME680 using I2C, use first device found
  {
    Serial.print(F("-  Unable to find BME680. Trying again in 5 seconds.\n"));
    delay(5000);
  } // of loop until device is located
  Serial.print(F("- Setting 16x oversampling for all sensors\n"));
  BME680.setOversampling(TemperatureSensor, Oversample16);           // Use enumerated type values to set value
  BME680.setOversampling(HumiditySensor, Oversample16);              // Use enumerated type values to set value
  BME680.setOversampling(PressureSensor, Oversample16);              // Use enumerated type values to set value
  Serial.print(F("- Setting IIR filter to a value of 4 samples\n"));
  BME680.setIIRFilter(IIR4); // Use enumerated type values
  Serial.print(F("- Turning off gas measurements\n"));
  BME680.setGas(0, 0);                                               // Setting either to 0 turns off gas measurement
  BME680.getSensorData(temperature, humidity, start_pressure, gas);  // Get most recent readings
  /***************************************************************
  ** Initialize SD-Card for logging, if not found then continue **
  ***************************************************************/
  Serial.print(F("- Checking to see if SD Card connected\n"));
  if (!digitalRead(SD_CARD_SPI_CD_PIN))
  {
    sd_card_present = false; // if pin is high then no card inserted
  }
  else
  {
    uint8_t loopCounter = 10;                                          ///< countdown to 0 to detect car
    while (--loopCounter && !SD.begin(SD_CARD_SPI_CS_PIN))             //  Try to start card using SPI
    {
      Serial.print(F("-  Unable to find SD Card. Trying again in 5 seconds.\n"));
      for (uint8_t i = 0; i < 50; i++)
      {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(100);
      } // loop to toggle LED light 10 times
    } // of loop until device is located
    if (loopCounter) sd_card_present = true;
  } // if-then card is inserted
  if (!sd_card_present)
  {
    Serial.print(F("- No SD-Card detected, continuing\n"));
  }
  else
  {
    Serial.print(F("- SD-Card Initialized\n"));
    dataFile = SD.open(FILE_NAME, FILE_WRITE);        // Open the logfile for writing and position to end-of-file
    if (!dataFile)
    {
      Serial.print(F("Unable to open file \""));
      Serial.print(FILE_NAME);
      Serial.print(F("\" on SD-Card. Error. Skipping SD writes."));
      sd_card_present = false; // turn off card
    }
    else
    {
      Serial.print(F("- File \""));
      Serial.print(FILE_NAME);
      Serial.print(F("\" successfully opened. Appending data.\n"));
      dataFile.print("Counter,Seconds,SupplyVoltage,Temperature,Humidity,Pressure\n");
    } // if-then-else the file could be opened
  } // if-then-else SD-Card located
  /**********************************************
  ** Connect to the specified wireless network **
  **********************************************/
  Serial.print(F("- Connecting to Wireless network \""));
  Serial.print(WIFI_SSID);
  Serial.print(F("\"."));
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Connect to the specified WiFi router
  while (WiFi.status() != WL_CONNECTED)   // Loop until the connection is established
  {
    delay(1000);                          // wait one second before trying again
    Serial.print("."); 
  } // for-next connection not established
  WiFi.setHostname("BME680");             // Give our device a name
  // Show connection data
  Serial.print("\n- Open browser to IP address \"");
  Serial.print(WiFi.localIP());
  Serial.print("\"\n- or to \"http://BME680\" to view data.\n");
  server.on("/", handleRoot);       // Routine for root page call
  server.on("/readADC", handleADC); // Page called by AJAX
  server.begin();                   //Start server
  Serial.println("HTTP server started");
} // of method "setup()"

void handleRoot() 
{
  //===============================================================
  // This routine is executed when you open its IP in browser
  //===============================================================
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
} // of method "handleRoot()"

void handleADC()
{
  server.send(200, "text/plain", jsonData);                        // Send JSON to client ajax request
}

void loop()
{
  server.handleClient();          // Handle client requests
  if (millis() > next_log_millis)
  {
    next_log_millis = millis() + SD_LOG_INTERVAL; // set next timer
    getSensorData();                              // get the BME680 data
  } // if-then time to get a measurement
} // of method "loop()"

void getSensorData()
{
  Serial.print(".");
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));               // Toggle LED each loop
  BME680.getSensorData(temperature, humidity, pressure, gas); // Get the most recent readings from the BME680
  if (sd_card_present)                                        // Log data to the SD-Card, if present
  {
    dataFile.print(++loopCounter);
    dataFile.print(",");
    dataFile.print(millis() / 1000);
    dataFile.print(",");
    dataFile.print(VOLTAGE);
    dataFile.print(",");
    dataFile.print(temperature /  100.0, 2);
    dataFile.print(",");
    dataFile.print(humidity    / 1000.0, 2);
    dataFile.print(",");
    dataFile.println(pressure    /  100.0, 2);
    if (loopCounter % SD_FLUSH_INTERVAL == 0)
    {
      dataFile.flush();
      Serial.print("\nFlushed data to SD-Card\n");
    } // flush the buffer
  } // if-then SD card is present       
  jsonData = "{\"SupplyVoltage\":\"" + String(VOLTAGE) + "\", \"Temperature\":\"" +
    String(temperature / 100.0) + "\", \"Humidity\":\"" + String(humidity / 1000.0) +
    "\", \"Pressure\":\"" + String((start_pressure - pressure) / 100.0) + "\"}";
} // of method "getSensorData()"
