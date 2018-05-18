#include "wio_tracker.h"

WioTracker wio = WioTracker();

void setup() {
  wio.Power_On();
  SerialDebug.println("Power On!");
  while(!wio.init()){
    delay(1000);
    SerialDebug.println("Accessing network...");
  }
  SerialDebug.println("Initialize done...");

  bool ret = wio.waitForNetworkRegister();
  if(true == ret){
      SerialDebug.println("Network accessed!");
  }else {
      SerialDebug.println("Network failed!");
      return;
  }

}
void loop() {
  int rssi;
  wio.getSignalStrength(&rssi);
  SerialDebug.print("RSSI: ");
  SerialDebug.print(rssi);
  SerialDebug.print(" ");
   
  if(rssi == 99) {
    rssi = 9999;
  }
  else if(rssi == 0) {
    rssi = -113;
  }
  else if(rssi == 1) {
    rssi = -111;
  }
  else if(rssi >= 2 && rssi <= 30) {
    rssi = -109 + 2*(rssi-2);
  }
  else if(rssi > 30) {
    rssi = -51 + (rssi-30)/2;   // approximate
  }
  
  SerialDebug.print(rssi);
  SerialDebug.println("dBm");
  delay(1000);
}
