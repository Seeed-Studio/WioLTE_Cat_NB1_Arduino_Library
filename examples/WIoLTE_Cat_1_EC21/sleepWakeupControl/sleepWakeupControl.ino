#include "wio_tracker.h"


WioTracker wio = WioTracker();

void setup() {
  pinMode(wio.DTR_PIN, INPUT);

  SerialDebug.println("Begin...");
  wio.Power_On();
  while(false == wio.Check_If_Power_On()){
    SerialDebug.println("Waitting for module to alvie...");
    delay(1000);
  }  
  SerialDebug.println("Power On O.K!");
}

void loop() {
  SerialDebug.println("sleep...");
  while(!wio.module_sleep()){
    SerialDebug.println("Try to sleep...");
    delay(1000);    
  }
  SerialDebug.println("sleep...");
  delay(4000);
  SerialDebug.println("wakeup...");
  while(!wio.module_wakeup()){
    SerialDebug.println("Try to wakeup...");
    delay(1000);    
  }
  SerialDebug.println("wakeup...");
  delay(4000);
}