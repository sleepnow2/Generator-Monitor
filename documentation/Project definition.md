The goal of this project is to make a very basic genset monitoring application that basically copies the genset panel of an Onan 30EM 50R onto a remote app accessible from anywhere.

Goals:
- [ ] read analog gauges from the genset
- [ ] present the data over MQTT topic
- [ ] digest data from MQTT topic
- [ ] present data to user on app
- [ ] provide local terminal access to basic configuration commands

Stretch goals:
- [ ] encrypt data-stream E2E... preferably through pub private key methods, but symmetrical works.
- [ ] provide terminal over application through MQTT topic
- [ ] Adjustable Email Alerts
	- [ ] Start/Stop Alerts
	- [ ] Alerts for parameters being out of scope
	- [ ] Alerts for fault lamps activating?
- [ ] log data locally and provide historical data through MQTT 

Resources:
