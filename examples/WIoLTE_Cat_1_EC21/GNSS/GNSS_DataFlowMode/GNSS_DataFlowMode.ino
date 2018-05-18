#include "gnss.h"

// Instance of GNSS class
GNSS gnss = GNSS();

void setup(){	
	// Module power on
  gnss.Power_On();
  while(false == gnss.Check_If_Power_On()){
    SerialDebug.println("Waitting for module to alvie...");
    delay(1000);
  } 
  SerialDebug.println("\n\rPower On!");

  if(!(gnss.open_GNSS())){
    SerialDebug.println("\n\rGNSS init failed!");
    return;
  }

  SerialDebug.println("Open GNSS OK.");
  delay(2000);
}

void loop(){
  gnss.dataFlowMode();
  delay(2000);
}
