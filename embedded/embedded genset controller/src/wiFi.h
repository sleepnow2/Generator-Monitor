


#ifndef WIFI_GENSET_CONTROLLER_H
#define WIFI_GENSET_CONTROLLER_H

#include "main.h"
#include <WiFi.h>

enum WiFiMode {
  ACCESS_POINT_MODE,
  STATION_MODE,
};

struct WiFiData {
  WiFiMode mode = ACCESS_POINT_MODE;
  String ssid = "Genset_Setup_Netork";
  String password = "12345678";
  IPAddress ip = IPAddress(192, 168, 4, 1);
};

WiFiData initializeWifi();
WiFiData softAPMode();
WiFiData stationMode(WiFiData wiFiData);

#endif // WIFI_GENSET_CONTROLLER_H
