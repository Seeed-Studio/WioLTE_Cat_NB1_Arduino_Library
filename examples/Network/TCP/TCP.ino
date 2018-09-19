#include <ublox_sara_r4_network.h>
#include <config.h>

UBLOX_SARA_R4_Network ublox = UBLOX_SARA_R4_Network();

char *ip = "www.lambor.win";
char *port = "41234";

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	while(false == ublox.isAlive()){
		Log_info("Waitting for module to alvie...");
		delay(1000);
	}  
	Log_info("Power On O.K!");

	if(!ublox.initialAtCommands())
	{
		Log_error("Failed to initialize module.");
		return;
	}
	Log_info("Initialize module success");

	if(!ublox.network_Init(30000)) { 
		Log_error("Network initialize timeout.");
		return;
	}
	else{
		Log_info("Operator: ");
		Log_info(ublox._operator);
		Log_info("IP address: ");
		Log_info(ublox.ip_string);
	}
	Log_info("Network initialize done.");

	// if(!ublox.socketClose(0)) {
	// 	SerialDebug.println("Close socket error!");
	// 	return;
	// }
	if(!ublox.createSocket(TCP)) {
		Log_error("Create socket error!");
		return;
	}
	if(!ublox.udpSendTo(0, ip, port, "Hello!")) {
		Log_error("UDP sendto error!");
		return;
	}
					
	Log_info("Sent UDP message.");
		
}	

void loop() {
	char rtc[32] = {'\0'};
	ublox.GetRealTimeClock(rtc);
	Log_info("rtc:");
	Log_info(rtc);
	if(ublox.udpSendTo(0, ip, port, rtc)) {
		Log_info("Sent UDP message successfully.");
	}
	else{
		Log_info("Failed to Sent message.");
	}
	delay(2000);
	// AT_bypass();
}

