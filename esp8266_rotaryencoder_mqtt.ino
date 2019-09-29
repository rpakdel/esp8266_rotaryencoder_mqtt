#include <RotaryEncoder.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "myssid.h"

#define RE_CLK_PIN D1
#define RE_DT_PIN D2
//#define RE_SW_PIN D3

const char* MQTT_SERVER = "192.168.1.29";
const char* MQTT_TOPIC = "ESP8266Volume";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
RotaryEncoder encoder(RE_DT_PIN, RE_CLK_PIN);

void mqttReconnect() 
{
	// Loop until we're reconnected
	while (!mqttClient.connected()) 
	{
		Serial.print("Attempting MQTT connection...");
		// Create a random client ID
		String clientId = "ESP8266Client-";
		clientId += String(random(0xffff), HEX);
		if (mqttClient.connect(clientId.c_str())) 
		{
			Serial.println("connected");
			char msg[128];
			sprintf(msg, "{ \"Online\" : true, \"IP\" : \"%s\" }", WiFi.localIP().toString().c_str());
			mqttClient.publish(MQTT_TOPIC, msg);
		}
		else 
		{
			Serial.print("failed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}


void setupWifi() 
{

	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(MYSSID);

	WiFi.begin(MYSSID, MYPASSWORD);

	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}

	randomSeed(micros());

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void setup()
{
	Serial.begin(115200);

	setupWifi();

	mqttClient.setServer(MQTT_SERVER, 1883);
}
bool firstZero = true;
void loop()
{
	if(!mqttClient.connected()) 
	{
		mqttReconnect();
	}
	mqttClient.loop();

	encoder.tick();
	int8_t dir = encoder.getDirection();
	if (dir != 0)
	{
		//Serial.println(dir);
		char msg[32];
		sprintf(msg, "{ \"dir\" : %d }", dir);
		mqttClient.publish(MQTT_TOPIC, msg);
	}
}
