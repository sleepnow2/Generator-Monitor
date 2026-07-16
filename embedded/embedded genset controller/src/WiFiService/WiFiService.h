#ifndef WIFI_GENSET_CONTROLLER_H
#define WIFI_GENSET_CONTROLLER_H
#include "main.h"

namespace WiFiService {
	enum WiFiMode {
		ACCESS_POINT_MODE,
		STATION_MODE,
	};

	struct WiFiData {
		WiFiMode mode = ACCESS_POINT_MODE;
		String ssid = "Genset_Setup_Network";
		String password = "12345678";
		IPAddress ip_address = IPAddress(192, 168, 4, 255);
		IPAddress ip_gateway = IPAddress(192, 168, 4, 1);
		IPAddress ip_netmask = IPAddress(255, 255, 255, 0);
	};

	WiFiData init();
	WiFiData softAPMode();
	WiFiData stationMode(WiFiData wiFiData);
}

#endif // WIFI_GENSET_CONTROLLER_H
