#include <Arduino.h>
#include <ublox_sara_r4_mqtt.h>
#include <ublox_sara_r4_ethernet.h>
#include <config.h>

UBLOX_SARA_R4_Ethernet ublox = UBLOX_SARA_R4_Ethernet();

char *ip = "www.lambor.win";
char *port = "1883";

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	while(false == ublox.isAlive()){
		Log_info("Waitting for module to alvie...");
		delay(1000);
  }  
  SerialDebug.println("Power On O.K!");
	
	// if(!ublox.network_Init(30000)) { 
	// 	Log_error("Network initialize timeout.");
	// 	return;
	// }
	// else{
	// 	Log_info("Operator: ");
	// 	Log_info(ublox._operator);
	// 	Log_info("IP address: ");
	// 	Log_info(ublox.ip_string);
	// }
	Log_info("Network initialize done.");
		
}	

void loop() {

}

