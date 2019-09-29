# ESP8266 (NodeMcu) publishing rotary encoder direction via MQTT messages

* ESP8266 is Lolin NodeMCU V3
* Rotary pins don't really matter, just switch their order when initializing RotaryEncoder object
* MQTT messages are published to local Mosquitto server
* Intended to be used with MQTT client that can change volume of device
