#include <ublox_sara_r4.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

char *server = "www.lambor.win";
uint16_t port = 41234;
int sockId = -1;

void setup() {
	Log_info("Begin...");
	
	ublox.powerOn();
	Log_info("Waitting for module to alvie...");
	while(false == ublox.isAlive()){
		Log(".");
		delay(100);
	}  
	Logln("");

	Log_info("Initializing network..");
	if(!ublox.network_Init(30000)) { 
		Log_error("Network initialize timeout.");
		return;
	}
	Log_info("APN: " + String(ublox._apn));
	Log_info("Local IP: " + String(ublox.ip_string));
	Log_info("Operator: " + String(ublox._operator));
	Log_info("Network attached.");
	
	if(-1 == (sockId = ublox.createSocket(UDP))) {
		Log_error("Create socket error!");
	}
	Log("[INFO] Create socket id: ");
	Logln(sockId);
	ublox.enableDirectLinkMode();
	// if(!ublox.udpSendTo(sockId, ip, port, "Hello!")) {
	// 	Log_error("UDP sendto error!");
	// 	return;
	// }
	if(!ublox.sockConnect(sockId, server, port)) {
		Log_error("connect to server failed.");
	}
	Log_info("Sent UDP message.");


		
}	

void loop() {
	static uint8_t tries = 0;
	// char rtc[32] = {'\0'};

	// ublox.GetRealTimeClock(rtc);
	// Log("[RTC] ");
	// Logln(rtc);
	// if(ublox.udpSendTo(sockId, ip, port, rtc)) {
	// 	Log_info("Sent UDP message successfully.");
	// }
	// else{
	// 	Log_info("Failed to Sent message.");
	// }

	String str_msg = "ublox random number " + String(random(0,100));

	ublox.socketWrite((uint8_t *)str_msg.c_str(), (uint16_t)str_msg.length());
	Log_info("Send msg: " + str_msg);

	tries++;
	if(tries > 5) {
		if(ublox.sockClose(sockId)) {
			Log_info("Close socket.");
		}
		while(true) AT_bypass();
	}

	delay(2000);
}

