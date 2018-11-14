#include <ublox_sara_r4.h>
#include <UART_Interface.h>

Ublox_sara_r4 ublox = Ublox_sara_r4();

void setup() {  
  Log_info("Read battery level million volt...");
}

void loop() {
	uint16_t battery = ublox.batteryMillionVolt();
	Log("Battery level: ");
	Log(battery);
	Logln("mV");
	delay(1000);
}
