#include <ublox_sara_r4.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {  
  Log_info("Begin...");
  ublox.powerOn();
  while(false == ublox.isAlive()) {
    Log_info("Waitting for module to alvie...");
    delay(1000);
  }  
  Log_info("Power On O.K!");

  if(!ublox.initialAtCommands()) 
  {
    Log_error("Failed to initialzie module.");
  }

  Log_info("Set APN.");
  if(!ublox.setAPN("IP", "CMNBIOT1"))
  {
    Log_error("Join APN error!");
  }

  Log_info("Initialize network...");
  if(!ublox.network_Init())
  {
    Log_error("Network initialize failed!");
  }
  else 
  {		
    Log_info("Network attached.");
  }

  Log_info("APN: " + String(ublox._apn));
  Log_info("Local IP: " + String(ublox._str_ip));
  Log_info("Operator: " + String(ublox._operator));

  Log_info("Start AT commands loop");
  
}

void loop() {
  /* Debug */
  AT_bypass();
}
