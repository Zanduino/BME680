/*! @file ESP32FeatherWiFiDemo.ino

@section ESP32FeatherWiFiDemo_intro_section Description

Example program for using an Arduino ESP32 based system (this sketch was developed and tested on a
ESP32 Huzzah32 Feather board from https://www.adafruit.com/product/3405) along with a BME680
connected via I2C to monitor the temperature, pressure and humidity and report the values in a
dynamic chart on a web page hosted by the ESP32 and connected to a local network.

Prior to compiling the program, the contents of the include file "Authentication.h" need to be
updated to reflect the local WiFi network to use and the corresponding authentication code.

Once started, the IP-Address is set by the WiFi router and displayed on the serial output of the
ESP32, this IP address should then be entered as the URL in a web browser of a computer attached to
the same network and the data should be presented there, updated every 10 seconds.

The Bosch BME680 sensor measures temperature, pressure, humidity and air quality and is described at
https://www.bosch-sensortec.com/bst/products/all_products/BME680. The datasheet is available from
Bosch at https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680_DS001-11.pdf \n\n

The most recent version of the BME680 library is available at https://github.com/SV-Zanshin/BME680
and the documentation of the library as well as example programs are described in the project's wiki
pages located at https://github.com/SV-Zanshin/BME680/wiki. \n\n

The BME680 is an extremely small physical package that is so tiny as to be impossible to solder at
home, hence it will be used as part of a third-party breakout board. There are several such boards
available at this time, for example \n Company  | Link
-------  | ----------
Sparkfun | https://www.sparkfun.com/products/14570
BlueDot  | https://www.bluedot.space/sensor-boards/bme680/
Adafruit |
https://learn.adafruit.com/adafruit-BME680-humidity-barometric-pressure-temperature-sensor-breakout
\n\n

Bosch supplies sample software that runs on various platforms, including the Arduino family; this
can be downloaed at https://github.com/BoschSensortec/BSEC-Arduino-library . This software is part
of the Bosch "BSEC" (Bosch Sensortec Environmental Cluster) framework and somewhat bulky and
unwieldy for typical Arduino applications, hence the choice to make a more compact and rather less
abstract library.

The pressure reading needs to be adjusted for altitude to get the adjusted pressure reading. There
are numerous sources on the internet for formulae converting from standard sea-level pressure to
altitude, see the data sheet for the BME180 on page 16 of
http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf. Rather than put a floating-point
function in the library which may not be used but which would use space, an example altitude
computation function has been added to this example program to show how it might be done.

@section ESP32FeatherWiFiDemolicense License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version. This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should have
received a copy of the GNU General Public License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

@section ESP32FeatherWiFiDemoauthor Author

Written by https://github.com/SV-Zanshin

@section ESP32FeatherWiFiDemoversions Changelog

Version | Date       | Developer  | Comments
------- | ---------- | ---------- | ---------------------------------------------------------------
1.0.1   | 2020-07-04 | SV-Zanshin | Issue #25 - implement clang-formatting
1.0.0   | 2020-06-02 | SV-Zanshin | Ready to commit and publish as Issue #20
1.0.0b  | 2020-05-30 | SV-Zanshin | Initial coding
*/

#if !defined(ESP32)
#error This program is designed for the ESP32 platform and might not work on other platforms
#endif

/**************************************************************************************************
** Declare all include files required                                                            **
**************************************************************************************************/
#include <SD.h>          ///< Include the SD Card standard library
#include <SPI.h>         ///< Include the SPI standard library
#include <WebServer.h>   ///< WiFi Web Server library
#include <WiFi.h>        ///< ESP32 WiFi Library
#include <WiFiClient.h>  ///< WiFi Client library

#include "WebPageContents.h"  ///< Include external HTML and JavaScript definitions
#include "Zanshin_BME680.h"   ///< The BME680 sensor library

/**************************************************************************************************
** Declare all program constants                                                                 **
**************************************************************************************************/
/*************************************************************************
** Please change the "WIFI_SSID", "WIFI_PASSWORD", "SD_CARD_SPI_CS_PIN" **
** and "SD_CARD_SPI_CD_PIN" constants to match your installation        **
*************************************************************************/
const char*    FILE_NAME          = "/BME_680.csv";  ///< Filename on SD-Card
const char*    HOSTNAME           = "BME680";        ///< Give the device a name
const char*    WIFI_SSID          = "NetworkSSID";   ///< Network SSID for connection
const char*    WIFI_PASSWORD      = "Password";      ///< Network authentication code
const uint32_t SERIAL_SPEED       = 115200;          ///< Set the baud rate for Serial I/O
const uint8_t  POWER_PIN          = A13;             ///< Supply voltage through a divider
const uint8_t  LED_PIN            = 13;              ///< This pin is the on-board red LED
const uint8_t  SD_CARD_SPI_CS_PIN = 21;              ///< (general GPIO) on ESP32 for chip select
const uint8_t  SD_CARD_SPI_CD_PIN = A5;              ///< (general GPIO) on ESP32 for carrier detect
const uint32_t SD_LOG_INTERVAL    = 1000;            ///< Milliseconds between writes to SD-Card
const uint8_t  SD_FLUSH_INTERVAL  = 60;              ///< do a "flush" after this number of writes
const float    SEA_LEVEL_PRESSURE = 1013.25;         ///< Standard atmosphere sea level pressure
/**************************************************************************************************
** Declare all global variables                                                                  **
**************************************************************************************************/
WebServer    server(80);               ///< Instantiate a web server on port 80
BME680_Class BME680;                   ///< Create an instance of the BME680 class
File         dataFile;                 ///< Class for a SD-Card file
int32_t      temperature;              ///< BME680 temperature value
int32_t      humidity;                 ///< BME680 humidity value
int32_t      pressure;                 ///< BME680 pressure value
int32_t      gas;                      ///< BME680 gas resistance value
int32_t      start_pressure;           ///< Initial pressure reading
bool         sd_card_present = false;  ///< Switch set when SD-Card detected
String       jsonData;                 ///< JSON data string
uint16_t     loopCounter = 0;          ///< Counter for number of write operations since startup

/**************************************************************************************************
** Macro that returns the floating point voltage. The formula is the analog value of POWER_PIN.  **
** since this goes through a voltage divider the value is multiplied by 2. The register is 12-bit**
** (0-4095) so we divide by 4095 and then by the 3.3V reference voltage and, finally, by the ADC **
** reference voltage of 1.1V.                                                                    **
**************************************************************************************************/
#define VOLTAGE (analogRead(POWER_PIN) * 2 / 4095.0 * 3.3 * 1.1)

void setup() {
  /*!
  @brief    Arduino method called once at startup to initialize the system
  @details  This is an Arduino IDE method which is called first upon boot or restart. It is only
            called one time and then control goes to the main "loop()" method, from which control
            never returns. The BME680, then the SD-Card (if present, if not present then it is
            ignored), then the Wi-Fi connection are initialized and configured here.
  @return   void
  */
  pinMode(SD_CARD_SPI_CS_PIN, OUTPUT);        // Chip-Select pin for the SD Card as output
  pinMode(SD_CARD_SPI_CD_PIN, INPUT_PULLUP);  // Carrier detect pin for the SD Card as input
  digitalWrite(SD_CARD_SPI_CS_PIN, HIGH);     // Deselect device by writing HIGH to it
  pinMode(LED_PIN, OUTPUT);                   // Make the on-board LED an output pin
  Serial.begin(SERIAL_SPEED);                 // Start the Serial comms at specified speed
  /**********************
  ** Initialize BME680 **
  **********************/
  Serial.print("Starting ESP32FeatherWiFiDemo for BME680\n");
  Serial.print("- Initializing BME680 sensor\n");
  while (!BME680.begin(I2C_STANDARD_MODE)) {  // Start using I2C, use first device found
    Serial.print("-  Unable to find BME680. Trying again in 5 seconds.\n");
    delay(5000);
  }  // of loop until device is located
  Serial.print("- Setting 16x oversampling for all sensors\n");
  Serial.print("- Setting IIR filter to a value of 4 samples\n");
  Serial.print("- Turning off gas measurements\n");
  BME680.setOversampling(TemperatureSensor, Oversample16);
  BME680.setOversampling(HumiditySensor, Oversample16);
  BME680.setOversampling(PressureSensor, Oversample16);
  BME680.setIIRFilter(IIR4);
  BME680.setGas(0, 0);  // Setting either to 0 turns off gas
  BME680.getSensorData(temperature, humidity, start_pressure, gas);  // Get most recent readings
  /***************************************************************
  ** Initialize SD-Card for logging, if not found then continue **
  ***************************************************************/
  Serial.print("- Checking to see if SD Card connected\n");
  if (!digitalRead(SD_CARD_SPI_CD_PIN)) {  // if pin is high then no card is inserted
    sd_card_present = false;
  } else {
    uint8_t loopCounter = 10;                                 ///< countdown to 0 to detect card
    while (--loopCounter && !SD.begin(SD_CARD_SPI_CS_PIN)) {  //  Try to start card using SPI
      Serial.print("-  Unable to find SD Card. Trying again in 5 seconds.\n");
      for (uint8_t i = 0; i < 50; i++) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(100);
      }  // loop to toggle LED light 10 times
    }    // of loop until device is located
    if (loopCounter) sd_card_present = true;
  }  // if-then card is inserted
  if (!sd_card_present) {
    Serial.print("- No SD-Card detected, continuing\n");
  } else {
    Serial.print("- SD-Card Initialized\n");
    dataFile = SD.open(FILE_NAME, FILE_WRITE);  // Open the logfile for writing and position to EOF
    if (!dataFile) {
      Serial.print("Unable to open file \"");
      Serial.print(FILE_NAME);
      Serial.print("\" on SD-Card. Error. Skipping SD writes.");
      sd_card_present = false;  // turn off card
    } else {
      Serial.print("- File \"");
      Serial.print(FILE_NAME);
      Serial.print("\" successfully opened. Appending data.\n");
      dataFile.print("Seconds,SupplyVoltage,Temperature,Humidity,Altitude\n");
    }  // if-then-else the file could be opened
  }    // if-then-else SD-Card located
  /**********************************************
  ** Connect to the specified wireless network **
  **********************************************/
  Serial.print("- Connecting to Wireless network \"");
  Serial.print(WIFI_SSID);
  Serial.print("\".");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);    // Connect to the specified WiFi router
  while (WiFi.status() != WL_CONNECTED) {  // Loop until the connection is established
    delay(1000);                           // wait one second before trying again
    Serial.print(".");
  }                            // for-next connection not established
  WiFi.setHostname(HOSTNAME);  // Give our device a name
  // Show connection data
  Serial.print("\n- Open browser to IP address \"");
  Serial.print(WiFi.localIP());
  Serial.print("\"\n- or to \"http://BME680\" to view data.\n");
  server.on("/", handleRoot);        // Routine for root page call
  server.on("/readADC", handleADC);  // Page called by AJAX
  server.begin();                    // Start server
  Serial.print("HTTP server started\n");
  digitalWrite(LED_PIN, false);  // Turn the LED off
}  // of method "setup()"
void handleRoot() {
  /*!
  @brief    Called when a request is sent from a browser client
  @details  When the ESP32 gets an IP address and that address is entered in a browser then this
            ISR get called to handle that request. The response is to send the HTML page which is
            contained in the variable "MAIN_page" which is set in the "WebPageContents.h" file
  @return   void
  */
  String s = MAIN_page;              // Read HTML contents into a string
  server.send(200, "text/html", s);  // Send the string as HTML to the requester
}  // of method "handleRoot()"
void handleADC() {
  /*!
  @brief    Called when a "readADC"" request is sent from a browser client
  @details  When the browser client sends a GET request for a "readADC" this ISR is called, which
            then returns the current value in the jsonData string, which is continuously updated
            in the main loop
  @return   void
  */
  if (!sd_card_present)  // If no SD-Card is present then we need to get data,
  {                      // if there is an SD-Card then the data has already been fetched
    getSensorData();     // get the BME680 data
  }                      // if-then SD-Card not present
  server.send(200, "text/plain", jsonData);  // Send JSON to client ajax request
}  // of method "handleADC()"
void loop() {
  /*!
  @brief    Arduino method for the main program loop
  @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on
            repeating. The timed measurements from the BME680 are handled here, while the actual
            web page serving and responses are handled by the ISRs "handleADC()" and "handleRoot()"
  @return   void
  */
  static uint32_t next_log_millis;                 ///< Millis() value for next SD-Card write time
  server.handleClient();                           // Handle client requests
  if (millis() > next_log_millis) {                // if it is time to get another measurement
    next_log_millis = millis() + SD_LOG_INTERVAL;  // set next time to get a reading
    getSensorData();                               // get the BME680 data
  }                                                // if-then time to get a measurement
}  // of method "loop()"
void getSensorData() {
  /*!
  @brief    Function to read the BME680 data and optionally write it to the SD-Card
  @details  The BME680 data is read in and if an SD-Card is present the values are written to it.
            The "jsonData" string is also put together here so that a client request can
            immediately send the current data. This is done because the frequency with which the
            data is written to the SD-Card is higher than that of the web page refresh.
  @return   void
  */
  BME680.getSensorData(temperature, humidity, pressure, gas);  // Get readings
  float altitude = 44330.0 * (1.0 - pow(((float)pressure / 100) / SEA_LEVEL_PRESSURE, 0.1903));
  if (sd_card_present) {  // Log data to SD-Card, if present
    dataFile.print(millis() / 1000);
    dataFile.print(",");
    dataFile.print(VOLTAGE);
    dataFile.print(",");
    dataFile.print(temperature / 100.0, 2);
    dataFile.print(",");
    dataFile.print(humidity / 1000.0, 2);
    dataFile.print(",");
    dataFile.print(altitude, 2);
    dataFile.print("\n");
    if (++loopCounter % SD_FLUSH_INTERVAL == 0) {    // Flush every N-Iterations
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle LED before flushing buffer
      dataFile.flush();                              // flush pending writes to SD
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle LED after flushing buffer
      Serial.print("Flushed data to SD-Card\n");
    }  // flush the buffer
  }    // if-then SD card is present
  jsonData = "{\"SupplyVoltage\":\"" + String(VOLTAGE) + "\", \"Temperature\":\"" +
             String(temperature / 100.0) + "\", \"Humidity\":\"" + String(humidity / 1000.0) +
             "\", \"Altitude\":\"" + String(altitude) + "\"}";
}  // of method "getSensorData()"
