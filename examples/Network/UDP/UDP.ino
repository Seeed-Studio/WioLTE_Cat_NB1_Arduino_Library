#include <ublox_sara_r4.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

char *ip = "www.lambor.win";
char *port = "41234";
int sockId = -1;

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
	else{
		Log_info("APN: ");
		Log(ublox._apn);
		Log_info("IP Address: ");
		Log(ublox.ip_string);
		Log_info("Operator: ");
		Log(ublox._operator);
	}
	Log_info("Network initialize done.");

	if(-1 == (sockId = ublox.createSocket(UDP))) {
		Log_error("Create socket error!");
		return;
	}
	Log_info("Create socket as id: ");
	Log(sockId);
	if(!ublox.udpSendTo(0, ip, port, "Hello!")) {
		Log_error("UDP sendto error!");
		return;
	}
					
	Log_info("Sent UDP message.");


		
}	

void loop() {
	// char rtc[32] = {'\0'};
	// ublox.GetRealTimeClock(rtc);
	// Log_info("rtc:");
	// Log_info(rtc);
	// if(ublox.udpSendTo(0, ip, port, rtc)) {
	// 	Log_info("Sent UDP message successfully.");
	// }
	// else{
	// 	Log_info("Failed to Sent message.");
	// }
	delay(2000);
	AT_bypass();
}

