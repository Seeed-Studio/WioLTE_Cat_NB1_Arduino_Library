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
char* topic = "Heat";

void messageReceived(char* cb_topic, char* cb_msg) {
    Logln(PRE_FIX"incoming msg: ");
    Log("Topic: "); Logln(cb_topic);
    Log("Msg: "); Logln(cb_msg);
}

void setup() {
    Log_info("Begin...");

    ublox.powerOn();
    Log_info("Waitting for module to alive...");
    while (false == ublox.isAlive()) {
        Log(".");
        delay(100);
    }
    Logln();

    Log_info("Initializing network");
    if (!ublox.network_Init(30000)) {
        Log_error("Network initialize timeout.");
        return;
    }

    // Set MQTT server
    if (mqtt.setServer(server, port)) {
        Logln(PRE_FIX"Set mqtt server success.");
    } else {
        Log_error("Set mqtt server failed");
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

    // Set subscribe
    if (mqtt.subscribe("Heat")) {
        Logln(PRE_FIX"mqtt subscribe topic: " + String(topic));
    } else {
        Log_error("mqtt subscribe failed");
    }

    mqtt.onMessage(messageReceived);

}

void loop() {
    if (mqtt.loop()) {
        return;
    }

    if (mqtt.subscribe("Heat")) {
        Logln(PRE_FIX"mqtt subscribe topic: " + String(topic));
    } else {
        Log_error("mqtt subscribe failed");
    }

}

