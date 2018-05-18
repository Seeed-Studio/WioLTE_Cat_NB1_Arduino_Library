#include "gnss.h"


GNSS gnss = GNSS();

void setup() {
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

void loop() {
  if(gnss.getCoordinate()){
    SerialDebug.println();
    SerialDebug.print("GNSS: \r\n");

    // Output double type
    SerialDebug.print("Data type in double: ");
    SerialDebug.print(gnss.longitude, 6);
    SerialDebug.print(",");
    SerialDebug.println(gnss.latitude, 6);
    
    // Output char* type
    SerialDebug.print("Data type in string: ");
    SerialDebug.print(gnss.str_longitude);
    SerialDebug.print(",");
    SerialDebug.println(gnss.str_latitude);
  } else{
    SerialDebug.print("...");
  }
}
