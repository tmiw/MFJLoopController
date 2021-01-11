#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include "WebServerController.h"
#include "CapacitorController.h"
#include "PowerMonitor.h"
#include "AutoTuneController.h"
#include "config.h"

CapacitorController capController;
PowerMonitor powerMonitor;
AutoTuneController autoTuneController(&capController, &powerMonitor);
WebServerController webServerController(&capController, &powerMonitor, &autoTuneController);

void checkWifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(1);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(STASSID, STAPSK);
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    // Close all existing connections. If we disconnected, the connections are invalid anyway.
    webServerController.closeAll();
  }
}

void setup() {
  checkWifi();
  
  // FS init
  LittleFS.begin();
  
  capController.setup();
  powerMonitor.setup();
  webServerController.setup();
  autoTuneController.setup();
  
  // Set up OTA
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([]() {
    // Stop what we're doing, if anything.
    capController.setDirection(CapacitorController::NONE);
    capController.setSpeed(CapacitorController::IDLE);
    capController.process();
    powerMonitor.enableAdc(false);

    // Wait a few ms for the ADC to stop signaling
    delay(5);
    
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
      LittleFS.end();
    }
    
    //Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");

    // Remount FS
    LittleFS.begin();

    // Re-enable ADC
    powerMonitor.enableAdc(true);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      //Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      //Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      //Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      //Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      //Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

}

void loop() 
{
  checkWifi();
  
  // Handle OTA
  ArduinoOTA.handle();

  capController.process();
  powerMonitor.process();
  autoTuneController.process();
  webServerController.process();
}
