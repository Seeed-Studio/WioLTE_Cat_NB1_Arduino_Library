#include <Arduino.h>

#include <math.h>

#include <ublox_sara_r4.h>
#include <ublox_sara_r4_mqtt.h>
#include <UART_Interface.h>

MQTT mqtt;
Ublox_sara_r4 ublox = Ublox_sara_r4();

char *server = "www.lambor.win";
char *port = "1883";

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	while(false == ublox.isAlive()){
		Log_info("Waitting for module to alive...");
		delay(1000);
	}  
	Log_info("Power On success!");
	
	if(!ublox.network_Init(30000)) { 
		Log_error("Network initialize timeout.");
		return;
	}
	Log_info("Network initialize done.");

	mqtt.setServer("45.62.100.29", 1883); ? Log_info("Set mqtt server success.") : Log_error("Set mqtt server failed");
	mqtt.setWillTopic("Heat") ? Log_info("Set mqtt will topic success.") : Log_error("Set mqtt will topic failed");
	mqtt.setWillMessage("ublox n/r410 online") ? Log_info("Set mqtt will msg success.") : Log_error("Set mqtt will msg failed");
	mqtt.connect() ? Log_info("mqtt connect success.") : Log_error("mqtt connect failed");
	mqtt.subscribe("Heat") ? Log_info("mqtt subscribe success.") : Log_error("mqtt subscribe failed");
}	

void loop() {		
    mqtt.loop();
    // AT_bypass();
}

