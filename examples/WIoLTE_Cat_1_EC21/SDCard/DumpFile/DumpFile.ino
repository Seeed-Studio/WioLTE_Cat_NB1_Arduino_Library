/*
  SD card file dump
 
 This example shows how to read a file from the SD card using the
 SD library and send it over the SerialDebug port.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header
 
 created  22 December 2010  by Limor Fried
 modified 9 Apr 2012  by Tom Igoe
 modified 16 Aug 2017  by Lambor Fang
 
 This example code is in the public domain.
 	 
 */
#include <SD.h>

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 43;

void setup()
{
  SerialDebug.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    SerialDebug.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  SerialDebug.println("card initialized.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      SerialDebug.write(dataFile.read());
    }
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    SerialDebug.println("error opening datalog.txt");
  } 
}

void loop()
{
}

