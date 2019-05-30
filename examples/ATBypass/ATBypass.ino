#include <ublox_sara_r4.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox;

void setup() {  
  delay(200);
  Log_info("## AT bypass demo");
  
  ublox.powerOn();

  Log_info("Waitting for module to alvie...");
  if(!ublox.isAlive(10000))
  {
    Log_error("Check alive timeout");
    return;
  }

  if(!ublox.initialAtCommands()) 
  {
    Log_error("Failed to initialzie module."); 
    return;  
  }  
}

void loop() {
  /* Debug */
  AT_bypass();
}
