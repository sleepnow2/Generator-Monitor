#line 1 "WiFiService.cpp"
#include "WiFiService.h"
#include <esp_wifi.h>

namespace WiFiService {
	/*
		IP address interpreter. 
		TODO: Needs to have its inputs hardened to make sure that malformed input addreses aren't valid.
	*/
	IPAddress interperetIPAddress(String inp) {
		LOG_TRACE();
		uint8_t octet[4] = {0,0,0,0};
		uint8_t current_octet = 0;

		for (uint8_t i = 0; i < inp.length(); i++) {
			if (inp[i] == '.') {
				// if this is a deliminator, move on to the next octet
				current_octet++;
				if(current_octet == 4) 
					break;
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
	WiFiService::WiFiData init() {
		LOG_TRACE();
		// requres that the file system is initialized and mounted.
		LOG_INFO("Opening config file wifi.json");
		JsonDocument config;
		WiFiService::WiFiData wiFiData;

		if (WebFileServer::readConfig(config, "/configs/wifi.json")) {
			wiFiData.ssid = config["ssid"].as<String>();
			LOG_DEBUG("ssid = " + wiFiData.ssid);
			wiFiData.password = config["password"].as<String>();
			LOG_DEBUG("password = " + wiFiData.password);
			wiFiData.ip_address = interperetIPAddress(config["ip-address"].as<String>());
			LOG_DEBUG("ip-address = " + wiFiData.ip_address.toString());
			wiFiData.ip_gateway = interperetIPAddress(config["ip-gateway"].as<String>());
			LOG_DEBUG("ip-gateway = " + wiFiData.ip_gateway.toString());
			wiFiData.ip_netmask = interperetIPAddress(config["ip-netmask"].as<String>());
			LOG_DEBUG("ip-netmask = " + wiFiData.ip_netmask.toString());
			wiFiData.ip_dnsadr1 = interperetIPAddress(config["ip-dnsadr1"].as<String>());
			LOG_DEBUG("ip-dnsadr1 = " + wiFiData.ip_dnsadr1.toString());
			wiFiData.ip_dnsadr2 = interperetIPAddress(config["ip-dnsadr2"].as<String>());
			LOG_DEBUG("ip-dnsadr2 = " + wiFiData.ip_dnsadr2.toString());
			wiFiData.mode = WiFiService::STATION_MODE;

			// attempt to connect to the wifi and send back the data to any functions who need it.
			return stationMode(wiFiData);
		} else { // (config_file)
			// if there is no config file, report the error and continue on to soft AP mode.
			LOG_WARN("Failed to open web config file!");
			LOG_WARN("Starting the network in station mode.");
		}
		return softAPMode();
	}

	/*
		soft AP mode is purely for setting up config files.
		this mode of operation does not perform anything else other than setting up the files.
	*/
	WiFiService::WiFiData softAPMode() {
		LOG_TRACE();
		WiFi.disconnect(false, true);
		WiFiService::WiFiData defaults;
		// begin WiFi in access point mode.
		WiFi.softAPConfig(defaults.ip_address, defaults.ip_gateway, defaults.ip_netmask);
		WiFi.softAP(defaults.ssid, defaults.password, 11, 0, 1);
		esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_BW_HT20);
		esp_wifi_set_max_tx_power(4);
		LOG_WARN("There was an issue setting up your device. Please connect over wifi to change configuration files.");
		LOG_WARN("Please connect to http://"+WiFi.softAPIP().toString()+":"+FILE_MANAGER_PORT+"/ for the file system");
		return defaults;
	}

	/*
		station mode is the usual operation it should be in.
	*/
	WiFiData stationMode(WiFiData wiFiData) {
		LOG_TRACE();
		WiFi.disconnect(false, true);
		WiFi.config(
			wiFiData.ip_address, 
			wiFiData.ip_gateway, 
			wiFiData.ip_gateway, 
			wiFiData.ip_dnsadr1, 
			wiFiData.ip_dnsadr2
		);
		WiFi.begin(wiFiData.ssid, wiFiData.password);
		uint8_t attempts = 0;
		while (WiFi.status() != WL_CONNECTED && attempts < 20) {
			LOG_INFO("connecting...");
			delay(1000);
			attempts++;
		}
		if (WiFi.status() == WL_CONNECTED) {
			LOG_INFO("WiFi Connected");
			LOG_INFO("Open Filemanager with http://"+WiFi.localIP().toString()+":"+FILE_MANAGER_PORT+"/");
		} else {
			return softAPMode();
		}
		return wiFiData;
	}
}