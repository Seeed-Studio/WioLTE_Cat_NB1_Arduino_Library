#include "ethernet.h"

Ethernet eth = Ethernet();


// const char apn[10] = "CMNET";
const char apn[10] = "UNINET";
const char URL[100] = "mbed.org";
char http_cmd[100] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\r\n\r";
int port = 80;

int ret = 0;


void setup() {
  SerialDebug.println("Begin...");
  eth.Power_On();
  while(false == eth.Check_If_Power_On()){
    SerialDebug.println("Waitting for module to alvie...");
    delay(1000);
  }

  while(!eth.init()){
    delay(1000);
    SerialDebug.println("Accessing network...");
  }
  SerialDebug.println("Initialize done...");

  eth.join(apn);
  SerialDebug.print("\n\rIP: ");
  SerialDebug.print(eth.ip_string);

  if(eth.connect(URL, port, TCP)) 
  {
    eth.write(http_cmd); 
    while(SerialModule.available()){
        SerialDebug.write(SerialModule.read());
    }    
    eth.close(1);
  } 
  else {
    SerialDebug.println("Connect Error!");
  }
  
}

void loop() {
  /* Debug */
  AT_bypass();
}
