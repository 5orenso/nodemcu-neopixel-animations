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


const char* PACKAGE_NAME = "main_1_databus_toilet_xmas_tree";
const char* SOFTWARE_BIN_URL = "https://raw.githubusercontent.com/5orenso/nodemcu-neopixel-animations/master/bin/nodemcu/toilet-xmas/firmware.bin";

#define DEBUG false
#define VERBOSE true
#define DEEP_SLEEP false

#define PUBLISH_INTERVAL 30
#define SLEEP_DELAY_IN_SECONDS  30

#define NUMPIXELS_1 80
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
long startWomenSessionSwitch;

bool redLight, greenLight;
bool redLightWomen, greenLightWomen;


int startMenIndicator = 0;
int endMenIndicator = 0;
int start = 1;
int end = 7;
int startWomenIndicator = 8;
int endWomenIndicator = 8;
int startWomen = 9;
int endWomen = 15;


float factorGender = .3;
int colorWomenRed = (int)(227 * factorGender);
int colorWomenGreen = (int)(46 * factorGender);
int colorWomenBlue  = (int)(54 * factorGender);

int colorMenRed = (int)(0 * factorGender);
int colorMenGreen = (int)(0 * factorGender);
int colorMenBlue  = (int)(255 * factorGender);

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
    int switchWomenStatus = root["switchWomen"];

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
        for (int i = 0; i < 5; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorRedRed, colorRedGreen, colorRedBlue, start + startPos, end + startPos); // Red
            neopixelSet1.setRange(colorMenRed, colorMenGreen, colorMenBlue, startMenIndicator + startPos, endMenIndicator + startPos); // Red
        }
        redLight = true;
        greenLight = false;
        startSessionSwitch = millis();
        mqttUtil.publishKeyValueInt(client, "redLight", 1);
        mqttUtil.publishKeyValueInt(client, "greenLight", 0);

    } else if (switchStatus == 1) {
        if (VERBOSE) {
            Serial.println("Green light on!");
        }
        for (int i = 0; i < 5; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorGreenRed, colorGreenGreen, colorGreenBlue, start + startPos, end + startPos); // Red
            neopixelSet1.setRange(colorMenRed, colorMenGreen, colorMenBlue, startMenIndicator + startPos, endMenIndicator + startPos); // Red
        }
        redLight = false;
        greenLight = true;
        mqttUtil.publishKeyValueInt(client, "redLight", 0);
        mqttUtil.publishKeyValueInt(client, "greenLight", 1);
    }


    // Is it occupied or not?
    if (switchWomenStatus == 2) {
        if (VERBOSE) {
            Serial.println("Red light Women on!");
        }
        for (int i = 0; i < 5; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorRedRed, colorRedGreen, colorRedBlue, startWomen + startPos, endWomen + startPos); // Red
            neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos); // Red
        }
        redLightWomen = true;
        greenLightWomen = false;
        startWomenSessionSwitch = millis();
        mqttUtil.publishKeyValueInt(client, "redLightWomen", 1);
        mqttUtil.publishKeyValueInt(client, "greenLightWomen", 0);

    } else if (switchWomenStatus == 1) {
        if (VERBOSE) {
            Serial.println("Green light Women on!");
        }
        for (int i = 0; i < 5; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorGreenRed, colorGreenGreen, colorGreenBlue, startWomen + startPos, endWomen + startPos); // Red
            neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos); // Red
        }
        redLightWomen = false;
        greenLightWomen = true;
        mqttUtil.publishKeyValueInt(client, "redLightWomen", 0);
        mqttUtil.publishKeyValueInt(client, "greenLightWomen", 1);
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

        if (redLight) {
            if (now - startSessionSwitch > (1000 * 60 * 30)) { // > 30 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 30, 0, start + startPos, end + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 30, 0, start1, end1);
            } else if (now - startSessionSwitch > (1000 * 60 * 20)) { // > 20 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, start + startPos, end + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, start1, end1);
            } else if (now - startSessionSwitch > (1000 * 60 * 10)) { // > 10 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, start + startPos, end + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, start1, end1);
            } else if (now - startSessionSwitch > (1000 * 60 * 5)) { // > 5 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, start + startPos, end + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, start1, end1);
            } else {
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.randomSparkRange(200, 200, 200, colorRedRed, colorRedGreen, colorRedBlue, 5, start + startPos, end + startPos);
                }
            }
        } else if (greenLight) {
            for (int i = 0; i < 5; i++) {
                int startPos = (i * (endWomen + 1));
                neopixelSet1.randomSparkRange(200, 200, 200, colorGreenRed, colorGreenGreen, colorGreenBlue, 5, start + startPos, end + startPos);
            }

        }

        if (redLightWomen) {
            if (now - startWomenSessionSwitch > (1000 * 60 * 30)) { // > 30 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 30, 0, startWomen + startPos, endWomen + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 30, 0, startWomen1, endWomen1);
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 20)) { // > 20 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, startWomen + startPos, endWomen + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, startWomen1, endWomen1);
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 10)) { // > 10 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, startWomen + startPos, endWomen + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, startWomen1, endWomen1);
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 5)) { // > 5 min session
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, startWomen + startPos, endWomen + startPos);
                }
                //     neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, startWomen1, endWomen1);
            } else {
                for (int i = 0; i < 5; i++) {
                    int startPos = (i * (endWomen + 1));
                    neopixelSet1.randomSparkRange(200, 200, 200, colorRedRed, colorRedGreen, colorRedBlue, 5, startWomen + startPos, endWomen + startPos);
                }
            }
        } else if (greenLightWomen) {
            for (int i = 0; i < 5; i++) {
                int startPos = (i * (endWomen + 1));
                neopixelSet1.randomSparkRange(200, 200, 200, colorGreenRed, colorGreenGreen, colorGreenBlue, 5, startWomen + startPos, endWomen + startPos);
            }

        }
    }
    if (now - lastLoop2 > 100) {
        lastLoop2 = now;

    }
    if (now - lastLoop3 > 200) {
        lastLoop3 = now;

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
