#include <ublox_sara_r4.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {
  
  SerialDebug.println("Begin...");
  ublox.powerOn();
  while(false == ublox.Check_If_Power_On()){
    SerialDebug.println("Waitting for module to alvie...");
    delay(1000);
  }  
  SerialDebug.println("Power On O.K!");

  delay(100);
  check_with_cmd("AT+UGPIOC=23,10\r\n", "OK", CMD);
  check_with_cmd("AT+UGPIOC=16,2\r\n", "OK", CMD);
}

void loop() {
  /* Debug */
  AT_bypass();
}
