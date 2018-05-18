#include <ublox_sara_r4.h>
#include <UART_Interface.h>
#include <Wire.h>

/**
 * In this sketch, we use Grove Uart to pass by massage from SeiralUSB,
 * to test this example, you can connect Grove GPS or other Serial device 
 * to the Grove UART port, after uploading this sketch, open Serial Monitor
 * then go on operating.
 * 
 * Notice: before using Grove UART/GPIO/I2C/Analog sockets, need to enable 
 *         its power at first. using Ublox_sara_r4::enableGrovePower();
 *         Power supply on D38 is always on.
*/

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {
  SerialGrove.begin(9600);
  SerialDebug.println("Begin...");  
  ublox.enableGrovePower();


  //Attach interrupt to Gpio D38, set D20 output follow D38
  pinMode(20, OUTPUT);
  pinMode(38, INPUT);
  attachInterrupt(38, GpioTest, CHANGE);

  //I2C test - Scan I2C device at Grove I2C socket
  i2cScan();

  //Analog test at A4 A5 A6 A7
  pinMode(4, INPUT_ANALOG);
  pinMode(5, INPUT_ANALOG);
  pinMode(6, INPUT_ANALOG);
  pinMode(7, INPUT_ANALOG);
  SerialDebug.print("Read A4: ");
  SerialDebug.println(analogRead(4));  
  SerialDebug.print("Read A5: ");
  SerialDebug.println(analogRead(5));
  SerialDebug.print("Read A6: ");
  SerialDebug.println(analogRead(6));
  SerialDebug.print("Read A7: ");
  SerialDebug.println(analogRead(7));

}

void loop() {
  /* Debug */
  while(SerialGrove.available()) {
    SerialDebug.write(SerialGrove.read());
  }
  while(SerialDebug.available()) {
    SerialGrove.write(SerialDebug.read());
  }
}

void i2cScan(void)
{
  uint8_t address;
  
  Wire.begin();
  for(address = 0; address < 127; address++) {
    Wire.beginTransmission(address);
    if(0 == Wire.endTransmission()) {
      SerialDebug.print("Detected i2c device at 0x");
      SerialDebug.println(address, HEX);
    }
  }
} 

void GpioTest(void)
{
  digitalWrite(20, digitalRead(38));
}