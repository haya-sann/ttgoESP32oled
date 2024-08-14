#include <Arduino.h>


//please select TTGO-LoRa32-OLED board
// from tutorial for TTGO esp32board with oled display
// https://electricnoodlebox.wordpress.com/tutorials/esp32-ttgo-dev-board-with-oled-display-tutorial/
//

// TTGO Test. This is a test script for a "TTGO" ESP32 Dev Board
// including an LCD module and a 18650 battery pack.
//
// There is an integrated LED on digital pin 16 - woohoo!!
//
// The built in OLED is a 128*64 mono pixel display
// i2c address = 0x3c
// SDA = 5
// SCL = 4
//
// I use the ThingPulse ESP8266 OLED library, found here:
// https://github.com/ThingPulse/esp8266-oled-SSD1306
//
// Please see the library for more detail on OLED commands
//
// Aidan Taylor
// March 2018
// Electric Noodle Box
#include <WiFiManager.h> // WiFiManagerライブラリのインクルード
#include "SSD1306Wire.h" // Include the correct library for ESP32
#include <Wire.h>
#include <time.h>

uint8_t ledPin = 16; // Onboard LED reference
SSD1306Wire display(0x3c, 5, 4); // Create an instance for the OLED. Addr, SDA, SCL
bool ledState = false; // LEDの状態を保持するフラッグ

void setup() {
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output

  Serial.begin(115200); // Start serial communication at 115200 baud
  Serial.println();
  Serial.println("Started Serial Communication");
  Serial.println("This Program is located at /Users/haya/Library/CloudStorage/Dropbox/reserve/DevDoc/Arduino/ESP32/TTGO/ttgoESP32oled/");
  Serial.println("This Program is located at /Users/haya/Documents/PlatformIO/Projects/ttgoESP32oled");
  display.init();                    // Initialize the OLED display
  display.flipScreenVertically();    // Flip the screen vertically
  display.setFont(ArialMT_Plain_24); // Set the font to ArialMT_Plain_24

  // WiFiManagerの初期化
//WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wifiManager.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
// Wi-Fi接続後にNTPサーバーから時刻を取得

configTime(0, 0, "pool.ntp.org"); // Configure NTP server

// Wait for successful connection to NTP server
while (time(nullptr) < 100000) {
  delay(1000);
}
// Set time zone to Japan Standard Time (JST)
setenv("TZ", "JST-9", 1);
tzset();
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  
  // Loopを繰り返す毎にLEDの状態を切り替える
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);


  time_t now = time(nullptr); // Get the current time
  struct tm* timeinfo = localtime(&now); // Convert to local time

  char buffer[16];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo); // Format time as HH:MM:SS

  display.clear(); // Clear the display
  display.drawString(0, 0, "Now the TIME"); // Draw the time string
  display.drawString(0, 40, buffer); // Draw the time string
  display.display(); // Display the updated content

  delay(1000); // Wait for 1 second
}