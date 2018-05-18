#include "gnss.h"


char nmea_sentence[192];
char nmea_GSV_sentence[512];
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
  gnss.enable_NMEA_mode();  // Set output sentence in NMEA mode
}

void loop() {  
  clean_buffer(nmea_sentence, 192);
  gnss.read_NMEA(GGA, nmea_sentence);
  SerialDebug.print(nmea_sentence);

  clean_buffer(nmea_sentence, 192);
  gnss.read_NMEA(RMC, nmea_sentence);
  SerialDebug.print(nmea_sentence);

  clean_buffer(nmea_sentence, 192);
  gnss.read_NMEA(GSA, nmea_sentence);
  SerialDebug.print(nmea_sentence);

  clean_buffer(nmea_sentence, 192);
  gnss.read_NMEA(VTG, nmea_sentence);
  SerialDebug.print(nmea_sentence);

  clean_buffer(nmea_GSV_sentence, 512);
  gnss.read_NMEA_GSV(nmea_sentence);
  SerialDebug.print(nmea_sentence);
  
  SerialDebug.println("\r\n");

  delay(1000);
}
