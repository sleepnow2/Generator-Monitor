#ifndef EMBEDDED_GENSET_CONTROLLER_MAIN_H
#define EMBEDDED_GENSET_CONTROLLER_MAIN_H
/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEFINES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
// defines the serial terminal to something slightly more professional looking.
#define ARDUINOJSON_ENABLE_COMMENTS 1
#define EMAIL_ENABLE_INTERNAL_SSLCLIENT 1


#define _LOG(level, text) \
    Serial.printf("[%s][%6u][%s:%u] %s(): ", level, millis(), __FILE__, __LINE__, __FUNCTION__);\
    Serial.println(text)
// log levels [error = 0, warn = 1, info = 2, debug = 3, trace =  4]
#define LOG_LEVEL 3
#define LOG_ERROR(text) _LOG("E", text) // error will always exist
#if LOG_LEVEL > 0
#define LOG_WARN(text) _LOG("W", text)
#else
#define LOG_WARN(text)
#endif
#if LOG_LEVEL > 1
#define LOG_INFO(text) _LOG("I", text)
#else
#define LOG_INFO(text)
#endif
#if LOG_LEVEL > 2
#define LOG_DEBUG(text) _LOG("D", text)
#else
#define LOG_DEBUG(text)
#endif
#if LOG_LEVEL > 3
#define LOG_TRACE(text) _LOG("T", text)
#else
#define LOG_TRACE(text)
#endif

/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Libraries !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
#include <Arduino.h>
#include <WiFi.h>         // for connection to the wifi
#include <EMailSender.h>  // for sending emails over SMTP to the users
// this allows for me to read json files in a basic way. This allows for config files to be written in a very human readable way.
#include <ArduinoJson.h>
#include <FS.h>           // basic file system interactions agnostic of actual FS used.
#include <FFat.h>         // the actual file system implementation chosen. This is because the web file manager did not like SPIFFS.
#include <ESPFMfGK.h>     // offers a wifi portal to edit files and folders inside the flash

/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Local Libs !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/
#include "WiFiService/WiFiService.h"        // offers basic types and definitions for custom wifi connection logic.
#include "webFileServer/webFileServer.h"    // handles the file IO both internal and external.
#include "emailService/emailService.h"      // handles everything email related. 
/************************************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! GLOBALS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************************************/

void setup();
void loop();

#endif // EMBEDDED_GENSET_CONTROLLER_MAIN_H