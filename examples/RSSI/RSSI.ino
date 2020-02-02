#include <ublox_sara_r4.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {

    SerialDebug.println("Begin...");
    ublox.powerOn();
    SerialDebug.print("Waitting for module to alvie...");
    while (false == ublox.isAlive()) {
        SerialDebug.print(".");
        delay(100);
    }

    while (!ublox.network_Init(30000)) {
        Log_error("Network initialize timeout.");
    }
    Log_info("APN: " + String(ublox._apn));
    Log_info("Local IP: " + String(ublox._str_ip));
    Log_info("Operator: " + String(ublox._operator));
    Log_info("Network attached.");
}

void loop() {
    int signal;
    if (ublox.getSignalStrength(&signal)) {
        SerialDebug.print("RSSI: ");
        SerialDebug.println(signal, DEC);
    } else {
        SerialDebug.print("Error");
    }

    delay(1000);

}
