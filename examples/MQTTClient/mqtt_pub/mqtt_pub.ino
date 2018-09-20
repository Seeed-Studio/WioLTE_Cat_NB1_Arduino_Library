#include <Arduino.h>

#include <math.h>

#include <ublox_sara_r4.h>
#include <ublox_sara_r4_mqtt.h>
#include <UART_Interface.h>

MQTT mqtt;
Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	Log("Waitting for module to alive...");
	while(false == ublox.isAlive()){
		Log(".");
		delay(200);
	}  
	Log_info("Power On success!");
	
	if(!ublox.network_Init()) { 
		Log_error("Network initialize timeout.");
		return;
	}
	Log_info("Network initialize done.");

	// Set MQTT server 
	if(!mqtt.setServer("www.lambor.win", 1883)) {
		Log_error("Set MQTT server failed");
		return;
	} else {
		Log_info("Set MQTT server success.");
	}

	// Set will topic
	if(!mqtt.setWillTopic("Heat")) {
		Log_error("Set MQTT will topic failed");
		return;
	} else {
		Log_info("Set MQTT will topic success.");
	}

	// Set will message
	if(!mqtt.setWillMessage("ublox n/r410 online")) {
		Log_error("Set MQTT will msg failed");
		return;
	} else {
		Log_info("Set MQTT will msg success.");
	}

	// Connect to server
	if(!mqtt.connect()) {
		Log_error("MQTT connect failed");
		return;
	} else {
		Log_info("MQTT connect success.");
	}
}	

void loop() 
{		
	if(!mqtt.connect()) {
		return;
	}
	
	static uint8_t tries = 0;
	String msg = String(random(2000, 3000)*1.0/100.0) + " degree";
	char *topic = "Heat";
		
	if(mqtt.publish(topic, msg.c_str())) {
		Log_info("MQTT published Topic " + String(topic) + "Messagea" + msg);	
	} else {
		Log_error("MQTT publish failed");
		while(true);
	}

	tries++;
	if(tries > 5)
	{
		if(mqtt.disconnect()) {
			Log_info("MQTT disconnect.");
		}
		while(true);
	}

	delay(2000);
}

