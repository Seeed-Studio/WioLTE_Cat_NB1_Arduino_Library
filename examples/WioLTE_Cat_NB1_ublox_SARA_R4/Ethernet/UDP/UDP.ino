#include <ublox_sara_r4_ethernet.h>

UBLOX_SARA_R4_Ethernet ublox = UBLOX_SARA_R4_Ethernet();

char *ip = "116.77.208.236";
char *port = "50001";

void setup() {
	SerialDebug.println("Begin...");
	
	ublox.powerOn();
	while(false == ublox.Check_If_Power_On()){
		SerialDebug.println("Waitting for module to alvie...");
		delay(1000);
  }  
  SerialDebug.println("Power On O.K!");
	
	if(!ublox.network_Init()) { 
		SerialUSB.println("Network initialize timeout.");
		return;
	}
	SerialDebug.println("Network initialize done.");

	// if(!ublox.socketClose(0)) {
	// 	SerialDebug.println("Close socket error!");
	// 	return;
	// }
	if(!ublox.createSocket(UDP, 1200)) {
		SerialDebug.println("Create socket error!");
		return;
	}
	if(!ublox.udpSendTo(0, ip, port, "Hello!")) {
		SerialDebug.println("UDP sendto error!");
		return;
	}
					
	SerialDebug.println("Sent UDP message.");
		
}	

void loop() {
	AT_bypass();
}

