#include <Adafruit_NeoPixel.h>
#include <NeoPixelAnimations.h>
#include <math.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MqttUtil.h>
#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>


const char* PACKAGE_NAME = "main_1_databus_toilet_light";
const char* SOFTWARE_BIN_URL = "https://raw.githubusercontent.com/5orenso/nodemcu-neopixel-animations/master/bin/nodemcu/toilet-light/firmware.bin";

#define DEBUG false
#define VERBOSE true
#define DEEP_SLEEP false

#define PUBLISH_INTERVAL 30
#define SLEEP_DELAY_IN_SECONDS  30

#define PIN_GREEN_LED D1
#define PIN_RED_LED D2
#define PIN_YELLOW_LED D3

#define NUMPIXELS_1 54
#define PIN_1 D7
#define TOTAL_EFFECTS 2
#define DELAY_BETWEEN_EFFECTS 10000


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const int   mqtt_port = MQTT_PORT;

const char* outTopic = MQTT_OUT_TOPIC;
const char* inTopic = MQTT_IN_TOPIC;

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
bool wifiConnected = false;
long wifiDisconnectedPeriode, wifiDisconnectedPeriodeStart;

WiFiClient espClient;
PubSubClient client(espClient);
long lastRun = millis();
char msg[150];
int nodemcuChipId = ESP.getChipId();

MqttUtil mqttUtil = MqttUtil(nodemcuChipId, PACKAGE_NAME, ssid, inTopic, outTopic, false);

Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);

// Timers
int prevTime,
    lastLoop1,
    lastLoop2,
    lastLoop3;

long startSessionSwitch;

bool teliaLight = true;
bool redLight, yellowLight, greenLight;

int startYellow = 0;
int endYellow = 9;

int startRed = 9;
int endRed = 45;

int startGreen = 9;
int endGreen = 45;

int startTelia = 45;
int endTelia = 54;

float factorYellow = .3;
int colorYellowRed = (int)(255 * factorYellow);
int colorYellowGreen = (int)(165 * factorYellow);
int colorYellowBlue = (int)(0 * factorYellow);

float factorTelia = .4;
int colorTeliaRed = (int)(227 * factorTelia);
int colorTeliaGreen = (int)(46 * factorTelia);
int colorTeliaBlue = (int)(54 * factorTelia);

float factorRed = .3;
int colorRedRed = (int)(255 * factorRed);
int colorRedGreen = (int)(0 * factorRed);
int colorRedBlue = (int)(0 * factorRed);

float factorGreen = .3;
int colorGreenRed = (int)(0 * factorGreen);
int colorGreenGreen = (int)(255 * factorGreen);
int colorGreenBlue = (int)(0 * factorGreen);

void setupWifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // OTA wifi setting
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("."); Serial.print(ssid);
        delay(500);
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    if (DEBUG) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
    }
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject((char*)payload);
    if (!root.success()) {
        if (VERBOSE) {
            Serial.println("handleUpdate: payload parse FAILED");
        }
        return;
    }
    // const char* incomingChipId = root["chipId"];
    int incomingChipId = root["chipId"];
    int switchStatus = root["switch"];
    int motionStatus = root["motion"];

    // Software update?
    Serial.print("Message for incomingChipId: "); Serial.println(incomingChipId);
    if (incomingChipId == nodemcuChipId) {
        Serial.print("Message for this cipdId: "); Serial.println(incomingChipId);
        int doUpdate = root["doUpdate"];
        Serial.print("doUpdate: "); Serial.println(doUpdate);
        if (doUpdate == 1) {
            Serial.println("Fetching firmware.bin...");
            t_httpUpdate_return ret = ESPhttpUpdate.update(SOFTWARE_BIN_URL, "", "21 99 13 84 63 72 17 13 B9 ED 0E 8F 00 A5 9B 73 0D D0 56 58");
            switch(ret) {
                case HTTP_UPDATE_FAILED:
                    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;
                case HTTP_UPDATE_NO_UPDATES:
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;
                case HTTP_UPDATE_OK:
                    Serial.println("HTTP_UPDATE_OK");
                    break;
            }
        }
    }

    if (DEBUG) {
        Serial.println("handleUpdate payload:"); root.prettyPrintTo(Serial); Serial.println();
        Serial.print("chipId: "); Serial.print(nodemcuChipId); Serial.print(" switch: "); Serial.println(switchStatus);
    }

    // Is it occupied or not?
    if (switchStatus == 2) {
        if (VERBOSE) {
            Serial.println("Red light on!");
        }
        digitalWrite(PIN_GREEN_LED, 0);
        digitalWrite(PIN_RED_LED, 1);
        // neopixelSet1.setRange(colorTeliaRed, colorTeliaGreen, colorTeliaBlue, startTelia, endTelia); // Telia Purple
        neopixelSet1.setRange(colorRedRed, colorRedGreen, colorRedBlue, startRed, endRed); // Red
        redLight = true;
        greenLight = false;
        startSessionSwitch = millis();
        mqttUtil.publishKeyValueInt(client, "redLight", 1);
        mqttUtil.publishKeyValueInt(client, "greenLight", 0);

    } else if (switchStatus == 1) {
        if (VERBOSE) {
            Serial.println("Green light on!");
        }
        digitalWrite(PIN_GREEN_LED, 1);
        digitalWrite(PIN_RED_LED, 0);
        float factor = .3;
        // neopixelSet1.setRange(colorTeliaRed, colorTeliaGreen, colorTeliaBlue, startTelia, endTelia); // Telia Purple
        neopixelSet1.setRange(colorGreenRed, colorGreenGreen, colorGreenBlue, startGreen, endGreen); // Green
        redLight = false;
        greenLight = true;
        mqttUtil.publishKeyValueInt(client, "redLight", 0);
        mqttUtil.publishKeyValueInt(client, "greenLight", 1);
    }

    // Are someone in line
    if (motionStatus == 2) {
        if (VERBOSE) {
            Serial.println("Yellow light on!");
        }
        digitalWrite(PIN_YELLOW_LED, 1);
        neopixelSet1.setRange(colorYellowRed, colorYellowGreen, colorYellowBlue, startYellow, endYellow); // Yellow
        yellowLight = true;
        mqttUtil.publishKeyValueInt(client, "yellowLight", 1);

    } else if (motionStatus == 1) {
        if (VERBOSE) {
            Serial.println("Yellow light off!");
        }
        digitalWrite(PIN_YELLOW_LED, 0);
        neopixelSet1.setRange(0, 0, 0, 0, 9);
        yellowLight = false;
        mqttUtil.publishKeyValueInt(client, "yellowLight", 0);

    }
}

void reconnectMqtt(uint32 ipAddress, long wifiDisconnectedPeriode) {
    while (!client.connected()) {
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        Serial.print("Attempting MQTT connection: ["); Serial.print(clientId); Serial.print("] : ");
        if (client.connect(clientId.c_str())) {
            Serial.println("Connected to MQTT!");
            client.subscribe(inTopic);
            mqttUtil.sendControllerInfo(client, ipAddress, wifiDisconnectedPeriode);
        } else {
            Serial.print("failed, rc="); Serial.print(client.state()); Serial.println(" try again in 5 seconds...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
        wifiConnected = true;
        wifiDisconnectedPeriode = millis() - wifiDisconnectedPeriodeStart;
        Serial.print("Wifi connected, IP: "); Serial.println(WiFi.localIP());
    });
    disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
        wifiConnected = false;
        wifiDisconnectedPeriodeStart = millis();
        Serial.println("Wifi disconnected...");
    });
    pinMode(PIN_GREEN_LED, OUTPUT);
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(PIN_1, OUTPUT);
	pixels1.begin();
    prevTime = millis();
    neopixelSet1.setAll(0, 0, 0);
    setupWifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnectMqtt(WiFi.localIP(), wifiDisconnectedPeriode);
    }
    client.loop();

    long now = millis();

    if (now - lastLoop1 > 10) {
        lastLoop1 = now;
        if (teliaLight) {
            neopixelSet1.randomSparkRange(200, 200, 200, colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 5, startTelia, endTelia);
            // neopixelSet1.fadeInOutRange(colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 3, 100, startTelia, endTelia);
        }
        if (redLight) {
            if (now - startSessionSwitch > (1000 * 60 * 30)) { // > 30 min session
                neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 30, 0, startRed, endRed);
            } else if (now - startSessionSwitch > (1000 * 60 * 20)) { // > 20 min session
                neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, startRed, endRed);
            } else if (now - startSessionSwitch > (1000 * 60 * 10)) { // > 10 min session
                neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, startRed, endRed);
            } else if (now - startSessionSwitch > (1000 * 60 * 5)) { // > 5 min session
                neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, startRed, endRed);
            } else {
                neopixelSet1.randomSparkRange(200, 100, 100, colorRedRed, colorRedGreen, colorRedBlue, 10, startRed, endRed);
            }
        }
    }
    if (now - lastLoop2 > 100) {
        lastLoop2 = now;
        if (yellowLight) {
            neopixelSet1.runningLightRange(colorYellowRed, colorYellowGreen, colorYellowBlue, startYellow, endYellow, 2);
        }
        if (teliaLight) {
            // neopixelSet1.randomSparkRange(150, 150, 150, colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 10, startTelia, endTelia, randomSparkSettingsTelia);
            // neopixelSet1.fadeInOutRange(colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 3, 100, startTelia, endTelia);
        }
    }
    if (now - lastLoop3 > 200) {
        lastLoop3 = now;
        if (teliaLight) {
            // neopixelSet1.randomSparkRange(150, 150, 150, colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 10, startTelia, endTelia, randomSparkSettingsTelia);
            // neopixelSet1.fadeInOutRange(colorTeliaRed, colorTeliaGreen, colorTeliaBlue, 10, startTelia, endTelia);
        }
    }

    // Stuff to do at given time intervals.
    if (now - lastRun > (PUBLISH_INTERVAL * 1000)) {
        lastRun = now;

        if (DEEP_SLEEP) {
            Serial.print("Entering deep sleep mode for "); Serial.print(SLEEP_DELAY_IN_SECONDS); Serial.println(" seconds...");
            ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
        }
    }
}
