#include <Arduino.h>

#include <math.h>

#include <ublox_sara_r4.h>
#include <ublox_sara_r4_mqtt.h>
#include <UART_Interface.h>

#define PRE_FIX  "[MQTT] "

MQTT mqtt;
Ublox_sara_r4 ublox = Ublox_sara_r4();

char* server = "server name or IP";
uint16_t port = 1883;

void setup() {
    Log_info("Begin...");

    ublox.powerOn();
    Log_info("Waitting for module to alive...");
    while (false == ublox.isAlive()) {
        Log(".");
        delay(100);
    }
    Logln();

    Log_info("Initializing network...");
    if (!ublox.network_Init()) {
        Log_error("Network initialize timeout.");
        return;
    }

    // Set MQTT server
    if (!mqtt.setServer(server, port)) {
        Log_error("Set MQTT server failed");
        return;
    } else {
        Logln(PRE_FIX"Set MQTT server success.");
    }

    // Set will
    if (!mqtt.setWill("Heat", "ublox n/r410")) {
        Log_error("Set MQTT will failed");
        return;
    } else {
        Logln(PRE_FIX"Set MQTT will success.");
    }

    // Connect to server
    Logln(PRE_FIX"Connecting to server: " + String(server));
    while (!mqtt.connect()) {}
    Logln(CRLF PRE_FIX"Connected\n\r");
}

void loop() {
    static uint8_t tries = 0;
    const char* topic = "Heat";
    String msg = String(random(2000, 3000) * 1.0 / 100.0) + " degree";


    if (mqtt.publish(topic, msg.c_str())) {
        Logln(PRE_FIX" published Topic " + String(topic) + " Messagea " + msg);
    } else {
        Log_error("MQTT publish failed");
        // while(true);
    }

    tries++;
    if (tries > 5) {
        if (mqtt.disconnect()) {
            Logln(PRE_FIX"Disconnect.");
        }
        Log_info("Enter AT command loop");
        while (true) {
            AT_bypass();
        }
    }

    delay(2000);
}

