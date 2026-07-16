#ifndef EMBEDDED_GENSET_CONTROLLER_MAIN_H
#define EMBEDDED_GENSET_CONTROLLER_MAIN_H
/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Libraries !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
#include <Arduino.h>
#include <WiFi.h>         // for connection to the wifi
// #include <EMailSender.h>  // for sending emails over SMTP to the users
// this allows for me to read json files in a basic way. This allows for config files to be written in a very human readable way.
#include <ArduinoJson.h>
#include <FS.h>           // basic file system interactions agnostic of actual FS used.
#include <FFat.h>         // the actual file system implementation chosen. This is because the web file manager did not like SPIFFS.
#include <ESPFMfGK.h>     // offers a wifi portal to edit files and folders inside the flash

/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEFINES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
// defines the serial terminal to something slightly more professional looking.
#define FILE_MANAGER_PORT 8080
#define FILE_SYSTEM FFat
/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! GLOBALS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
extern ESPFMfGK filemgr;  

bool initializeFileSystem();
void setup();
void loop();

#endif // EMBEDDED_GENSET_CONTROLLER_MAIN_H