/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Libraries !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/

#include "main.h"      // the basic includes involved with nearly everyone.
#include "wiFi.h"      // offers basic types and definitions for custom wifi connection logic.
#include "webFileServer.h" // yuh

/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Main Functions !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
ESPFMfGK filemgr(FILE_MANAGER_PORT);


/*
  this initializes the file system so everything else can use it. we have a grad total of 9.9 MB to use.
*/
bool initializeFileSystem() {
    // initialize the file system so configuration files can be read.
  Serial.println("Initializing FileSystem");
  while (!FILE_SYSTEM.begin()) {
    Serial.println("Failiure to initialize filesystem.");
    Serial.println("Reformating file system.");
    FILE_SYSTEM.format();
    delay(1000);
  }
  Serial.println("Success initializing filesystem.");
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("hello!");
  delay(2000);
  Serial.println("Initializing system...");
  Serial.println("");
  Serial.flush();

  initializeFileSystem();
  WiFiData wiFiData = initializeWifi();
  setupFilemanager();
  if (wiFiData.mode == ACCESS_POINT_MODE) {
    uint64_t last_log = millis();
    // if we are in local setup mode, then do nothing else.
    while (1) {
      filemgr.handleClient();
      if ((millis() + 1000) > last_log) {
        Serial.print("ip address = ");
        Serial.print(WiFi.softAPIP());
        Serial.println(":8080");
        last_log = millis();
      }
      yield();
    }
  }
}

void loop() {
  filemgr.handleClient();
}