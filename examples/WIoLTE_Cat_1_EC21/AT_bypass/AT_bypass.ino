#include "wio_tracker.h"


WioTracker wio = WioTracker();

void setup() {
  SerialDebug.println("Begin...");
  wio.Power_On();
  while(false == wio.Check_If_Power_On()){
    SerialDebug.println("Waitting for module to alvie...");
    delay(1000);
  }  
  SerialDebug.println("Power On O.K!");
}

void loop() {
  /* Debug */
  AT_bypass();
}
