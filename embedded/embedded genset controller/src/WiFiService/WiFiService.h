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
		IPAddress ip_dnsadr1 = IPAddress(1, 1, 1, 1); // cloudfare public dns server 
		IPAddress ip_dnsadr2 = IPAddress(8, 8, 8, 8); // google public dns server
	};

	WiFiData init();
	WiFiData softAPMode();
	WiFiData stationMode(WiFiData wiFiData);
}

#endif // WIFI_GENSET_CONTROLLER_H
