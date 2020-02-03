#include <ublox_sara_r4.h>
#include <libmaple/pwr.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {
    SerialDebug.println("Begin...");
    ublox.powerOn();
    SerialDebug.println("Waitting for module to alvie...");
    while (false == ublox.isAlive()) {
        SerialDebug.print(".");
        delay(1000);
    }
    SerialDebug.println();
    SerialDebug.println("Power On O.K!");

    delay(100);
    check_with_cmd("AT+UGPIOC=23,10\r\n", "OK", CMD);
    check_with_cmd("AT+UGPIOC=16,2\r\n", "OK", CMD);

    SerialDebug.println("Input 's' to enter standby mode.");
}

void loop() {
    if (SerialDebug.available()) {
        char chr = SerialDebug.read();
        if (chr == 's') {
            pwr_enter_standby_mode();
        }
    }
}
