#include <ublox_sara_r4.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

char *server = "host";
uint16_t port = 1234;
int sockId = -1;

void setup() {
	bool network_attached = false;

	Log_info("Begin...");
	
	ublox.powerOn();
	Log_info("Waitting for module to alvie...");
	while(false == ublox.isAlive()) {
		Log(".");
		delay(100);
	}  
	Logln("");

	Log_info("Initializing network...");
  network_attached = ublox.network_Init(120);
	if(!network_attached) {    
		Logln();
		Log_error("Network initialize timeout.");
		while(1);
	}
	Log_info("APN: " + String(ublox._apn));
	Log_info("Local IP: " + String(ublox._str_ip));
	Log_info("Operator: " + String(ublox._operator));
	Log_info("Network attached.");
	
	if(-1 == (sockId = ublox.createSocket(UDP))) {
		Log_error("Create socket error!");
	}
	Log("[INFO] Create socket id: ");
	Logln(sockId);

	ublox.disableDirectLinkMode();

	Log_info("Sent UDP message.");
}	

void loop() {
	static uint8_t tries = 0;

    String str_msg = "ublox random number " + String(random(0,100));
    
	if(ublox.udpSendTo(sockId, server, port, (char *)str_msg.c_str(), (uint16_t)str_msg.length())) {
		Log_info("Send msg: " + str_msg);
	}
	else{
		Log_info("Failed to Sent message.");
	}
	
	tries++;
	if(tries > 5) {
		if(ublox.sockClose(sockId)) {
			Log_info("Close socket.");
		}
		while(true) AT_bypass();
	}

	delay(2000);
}

