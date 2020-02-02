
#include <Arduino.h>
#include <ublox_sara_r4.h>

Ublox_sara_r4 nbIoT = Ublox_sara_r4();

void setup() {

}

void loop() {
    Log_info("Start");
    delay(2000);

    nbIoT.SystemReset();
}
