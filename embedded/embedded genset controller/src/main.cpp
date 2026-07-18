#line 1 "main.cpp"
#include "main.h" // the basic includes involved with nearly everyone.

SemaphoreHandle_t SerialMutex = xSemaphoreCreateMutex();

void setup() {
	Serial.begin(115200);
	while (!Serial);
	LOG_DEBUG("hello!");
	delay(5000);
	LOG_INFO("Initializing system...");

	WebFileServer::init(true); // sets up the file system
	WiFiService::WiFiData wiFiData = WiFiService::init(); // sets up the wifi access point
	WebFileServer::begin(); // begins the website thread. This thread is not pinned to a core.
	
	if (wiFiData.mode == WiFiService::ACCESS_POINT_MODE) {
		uint64_t last_log = millis();
		// if we are in local setup mode, then do nothing else.
		for (;;) {
			LOG_INFO("ip address = " + WiFi.softAPIP().toString() + ":8080");
			delay(5000);
		}
	}

	LOG_INFO("test email service!");
	//EmailService::testEmailService1();
}


void loop() {
	LOG_INFO("ip address = " + WiFi.localIP().toString() + ":8080");
	delay(5000);
}