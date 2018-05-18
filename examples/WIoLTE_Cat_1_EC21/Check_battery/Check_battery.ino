
#include "UART_Interface.h"

const int battery_adc_Pin = 16;  // PB0

void setup() {

}

void loop() {
    float a = analogRead(battery_adc_Pin);
    float v = a*3300/2048;        
    SerialDebug.print("The voltage of battery is ");
    SerialDebug.print(v, 2);
    SerialDebug.println(" mV");
    delay(1000);
}