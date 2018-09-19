#include <Arduino.h>
#include <ublox_sara_r4.h>
#include <ublox_sara_r4_mqtt.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

char *server = "www.lambor.win";
char *port = "1883";

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	while(false == ublox.isAlive()){
		Log_info("Waitting for module to alvie...");
		delay(1000);
  }  
  SerialDebug.println("Power On O.K!");
	
	if(!ublox.network_Init(30000)) { 
		Log_error("Network initialize timeout.");
		return;
	}
	Log_info("Network initialize done.");

	mqtt.setServerByDomain(server);	
	mqtt.setWillTopic("Heat");
	mqtt.setWillMessage("lambor");
	mqtt.connect();
	mqtt.publish("Heat", "23 deg");
}	

void loop() {

}

