#include <Arduino.h>
// #include <config.h>
#include <UART_Interface.h>

void setup() {

    // Use D38 port as input
    pinMode(38, INPUT);
    attachInterrupt(38, isr, RISING);
    SerialDebug.println("Begin...");
}

void loop() {
    /* Debug */
    AT_bypass();
}

void isr(void) {
    SerialDebug.println("D38 rising interrupt");
}

