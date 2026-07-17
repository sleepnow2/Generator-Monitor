#line 1 "main.cpp"
#include "main.h" // the basic includes involved with nearly everyone.


void setup() {
	Serial.begin(115200);
	while (!Serial);
	LOG_DEBUG("hello!");
	delay(2000);
	LOG_INFO("Initializing system...");
	Serial.flush();

	WebFileServer::init(true);
	WiFiService::WiFiData wiFiData = WiFiService::init();
	WebFileServer::begin();
	if (wiFiData.mode == WiFiService::ACCESS_POINT_MODE) {
		uint64_t last_log = millis();
		// if we are in local setup mode, then do nothing else.
		while (1) {
			WebFileServer::handleClient();
			if (millis() > last_log + 5000) {
				LOG_INFO("ip address = " + WiFi.softAPIP().toString() + ":8080");
				last_log = millis();
			}
			delay(1);
		}
	}

	EmailService::testEmailService1();
}


void loop() {
	LOG_TRACE();
	static uint64_t last_log = millis();
	WebFileServer::handleClient();
	if (millis() > last_log + 5000) {
		//LOG_INFO("ip address = " + WiFi.localIP().toString() + ":8080");
		last_log = millis();
	}
	delay(100);
}