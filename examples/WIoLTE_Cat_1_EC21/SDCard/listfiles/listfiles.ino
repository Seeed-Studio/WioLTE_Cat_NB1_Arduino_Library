/*
  SD card basic file example
 
 This example shows how to create and destroy an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header

 created   Nov 2010 by David A. Mellis
 modified 9 Apr 2012 by Tom Igoe
 modified 13 June 2012 by Limor Fried
 modified 16 Aug 2017  by Lambor Fang
 
 This example code is in the public domain.
 	 
 */
#include <SD.h>

File root;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 43;

void setup()
{
  SerialDebug.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on Arduino Uno boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    SerialDebug.println("initialization failed!");
    return;
  }
  SerialDebug.println("initialization done.");

  root = SD.open("/");
  
  printDirectory(root, 0);
  
  SerialDebug.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  // Begin at the start of the directory
  dir.rewindDirectory();
  
  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //SerialDebug.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       SerialDebug.print('\t');   // we'll have a nice indentation
     }
     // Print the 8.3 name
     SerialDebug.print(entry.name());
     // Recurse for directories, otherwise print the file size
     if (entry.isDirectory()) {
       SerialDebug.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       SerialDebug.print("\t\t");
       SerialDebug.println(entry.size(), DEC);
     }
     entry.close();
   }
}



