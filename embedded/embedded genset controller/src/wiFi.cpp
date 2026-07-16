#include "wiFi.h"

void throwException(String inp) {
    Serial.print("exception. malformed ip address input:");
    Serial.println(inp);
    Serial.flush();
    delay(100);
    throw "nuhuh";
}
IPAddress interperetIPAddress(String inp) {
  uint8_t octet[4] = {192,168,4,1};
  /*
  uint8_t current_octet = 0;
  uint8_t i = 0;
  while (inp[i] != '\0') {
    if (inp[i] == '.') {
      // if this is a decimal, move on to the next octet
      current_octet++;
    } else {
      octet[current_octet] *= 10;
      octet[current_octet] += inp[i]-'0';
    }
    i++;
  }
  */
  return IPAddress(octet[0],octet[1],octet[2],octet[3]);
}


/* 
  reads the wifi.cfg file and begins wifi operations according to the cfg.
 
  if we fail to connect in ten seconds, we will instead default to wifi access point mode.
*/
WiFiData initializeWifi() {
  // requres that the file system is initialized and mounted.
  Serial.println("Opening config file wifi.json");
  File config_file = FILE_SYSTEM.open(F("/wifi.json"), "r");

  WiFiData wiFiData;
  if (config_file) {
    JsonDocument config;
    deserializeJson(config, config_file);

    wiFiData.ssid = config["ssid"].as<String>();
    wiFiData.password = config["password"].as<String>();
    wiFiData.ip = interperetIPAddress(config["ip-address"].as<String>());

    wiFiData.mode = STATION_MODE;

    // attempt to connect to the wifi and send back the data to any functions who need it.
    return stationMode(wiFiData);
  } else { // (config_file)
    // if there is no config file, report the error and continue on to soft AP mode.
    Serial.println("Failed to open config file! please use the terminal to set config files");
  }
  return softAPMode();
}

/*
  soft AP mode is purely for setting up config files.
  this mode of operation does not perform anything else other than setting up the files.
*/
WiFiData softAPMode() {
  WiFiData defaults;
  // begin WiFi in access point mode.
  WiFi.softAPConfig(defaults.ip, IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  WiFi.softAP(defaults.ssid, defaults.password);
  Serial.println("There was an issue setting up your device. Please connect over wifi to change configuration files.");
  Serial.print("Please connect to http://");
  Serial.print(WiFi.softAPIP());
  Serial.print(":");
  Serial.print(8080);
  Serial.println("/ for the file system.");
  return defaults;
}

WiFiData stationMode(WiFiData wiFiData) {
  WiFi.config(wiFiData.ip, IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  WiFi.begin(wiFiData.ssid, wiFiData.password);
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED || attempts < 20) {
    delay(1000);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected");
    Serial.print(F("Open Filemanager with http://"));
    Serial.print(WiFi.localIP());
    Serial.print(F(":"));
    Serial.print(FILE_MANAGER_PORT);
    Serial.print(F("/"));
    Serial.println();
  } else {
    return softAPMode();
  }
  return wiFiData;
}
