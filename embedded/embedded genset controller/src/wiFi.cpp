#include "wiFi.h"


/*
  IP address interpreter. 
  TODO: Needs to have its inputs hardened to make sure that malformed input addreses aren't valid.
*/
IPAddress interperetIPAddress(String inp) {
  uint8_t octet[4] = {0,0,0,0};
  uint8_t current_octet = 0;
  uint8_t i = 0;

  for (uint8_t i = 0; i < inp.length(); i++) {
    if (inp[i] == '.') {
      // if this is a deliminator, move on to the next octet
      current_octet++;
    } else {
      octet[current_octet] *= 10;
      octet[current_octet] += inp[i]-'0';
    }
  }
  return IPAddress(octet[0],octet[1],octet[2],octet[3]);
}


/* 
  reads the wifi.cfg file and begins wifi operations according to the cfg.
 
  if we fail to connect in ten seconds, we will instead default to wifi access point mode.
*/
WiFiData initializeWifi() {
  // requres that the file system is initialized and mounted.
  Serial.println("Opening config file wifi.json");
  File config_file = FILE_SYSTEM.open(F("/configs/wifi.json"), "r");
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  WiFiData wiFiData;
  if (config_file) {
    Serial.println("success opening wifi folder, interpereting json.");
    JsonDocument config;
    String config_text = config_file.readString();
    Serial.println(config_text);
    DeserializationError err = deserializeJson(config, config_text);
    Serial.println(err.c_str());

    wiFiData.ssid = config["ssid"].as<String>();
    Serial.print("ssid = ");
    Serial.println(wiFiData.ssid);

    wiFiData.password = config["password"].as<String>();
    Serial.print("password = ");
    Serial.println(wiFiData.password);

    wiFiData.ip_address = interperetIPAddress(config["ip-address"].as<String>());
    Serial.print("ip-address = ");
    Serial.println(wiFiData.ip_address);

    wiFiData.ip_gateway = interperetIPAddress(config["ip-gateway"].as<String>());
    Serial.print("ip-gateway = ");
    Serial.println(wiFiData.ip_gateway);

    wiFiData.ip_netmask = interperetIPAddress(config["ip-netmask"].as<String>());
    Serial.print("ip-netmask = ");
    Serial.println(wiFiData.ip_netmask);

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
  WiFi.softAPConfig(defaults.ip_address, defaults.ip_gateway, defaults.ip_netmask);
  WiFi.softAP(defaults.ssid, defaults.password);
  Serial.println("There was an issue setting up your device. Please connect over wifi to change configuration files.");
  Serial.print("Please connect to http://");
  Serial.print(WiFi.softAPIP());
  Serial.print(":");
  Serial.print(8080);
  Serial.println("/ for the file system.");
  return defaults;
}

/*
  station mode is the usual operation it should be in.
*/
WiFiData stationMode(WiFiData wiFiData) {
  WiFi.config(wiFiData.ip_address, wiFiData.ip_gateway, wiFiData.ip_gateway);
  WiFi.begin(wiFiData.ssid, wiFiData.password);
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    Serial.println("connecting...");
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
