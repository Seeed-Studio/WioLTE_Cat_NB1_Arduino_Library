#include <ublox_sara_r4.h>
#include <UART_Interface.h>
#include <Wire.h>

/**
    In this sketch, we test all peripheral on the board.
    1. Grove UART pass through to USB port, you can connect Grove GPS to read message from USB port.
    2. External interrupt using pin D38, D20 follow the state of D38.
    3. Read ADC0 ~ ADC3
    4. I2C detect using Grove I2C port
    5. AT passthrough between USB port and cellular module.

    Notice: before using Grove UART/GPIO/I2C/Analog sockets, need to enable
           its power at first. using Ublox_sara_r4::turnOnGrovePower();
           Power supply on D38 is always on.
*/

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {
    SerialGrove.begin(9600);
    SerialDebug.println("Begin...");
    ublox.turnOnGrovePower();


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
    while (SerialGrove.available()) {
        SerialDebug.write(SerialGrove.read());
    }
    while (SerialDebug.available()) {
        SerialGrove.write(SerialDebug.read());
    }
}

void i2cScan(void) {
    uint8_t address;

    Wire.begin();
    for (address = 0; address < 127; address++) {
        Wire.beginTransmission(address);
        if (0 == Wire.endTransmission()) {
            SerialDebug.print("Detected i2c device at 0x");
            SerialDebug.println(address, HEX);
        }
    }
}

void GpioTest(void) {
    digitalWrite(20, digitalRead(38));
}