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

  Log_info("Start AT commands loop");
  
}

void loop() {
  /* Debug */
  AT_bypass();
}
