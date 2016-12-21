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

#include <TeliaWifiClient.h>

const char* PACKAGE_NAME = "main_1_databus_toilet_xmas_tree";
const char* SOFTWARE_BIN_URL = "https://raw.githubusercontent.com/5orenso/nodemcu-neopixel-animations/master/bin/nodemcu/toilet-xmas/firmware.bin";

#define DEBUG false
#define VERBOSE true
#define DEEP_SLEEP false

#define WIFI_RESET_TIMER_SECONDS 60
#define MQTT_RESET_TIMER_SECONDS 60

#define PUBLISH_INTERVAL 30
#define SLEEP_DELAY_IN_SECONDS  30

#define NUMPIXELS_1 80
#define PIN_1 D7
#define TOTAL_EFFECTS 2
#define DELAY_BETWEEN_EFFECTS 10000

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* teliaWifiUser = TELIA_WIFI_USER;
const char* teliaWifiPassword = TELIA_WIFI_PASSWORD;

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
    lastLoop3,
    lastLoop4,
    menLoop,
    womenLoop;

long startSessionSwitch, startSessionSwitchGreen, startWomenSessionSwitch, startWomenSessionSwitchGreen;

bool redLight, greenLight, yellowLight;
bool redLightWomen, greenLightWomen;
int menLightMode, menLightModeGreen, womenLightMode, womenLightModeGreen;

int numberOfSets = 5;

int menIndex = 1;
int womenIndex = 2;
int menIndicatorIndex = 3;
int womenIndicatorIndex = 4;

int startYellow = 0;
int endYellow = 2;

int startMenIndicator = 0;
int endMenIndicator = 2;
int start = 3;
int end = 7;
int startWomenIndicator = 8;
int endWomenIndicator = 10;
int startWomen = 11;
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

float factorYellow = .4;
int colorYellowRed = (int)(255 * factorYellow);
int colorYellowGreen = (int)(165 * factorYellow);
int colorYellowBlue = (int)(0 * factorYellow);


void setupWifi() {
    long loopTimer = millis();
    Serial.print("Connecting to "); Serial.println(ssid);
    WiFi.begin(ssid);
    while (WiFi.status() != WL_CONNECTED) {
        long now = millis();
        if (now - loopTimer > (WIFI_RESET_TIMER_SECONDS * 1000)) {
            ESP.restart();
        }
        Serial.print("."); Serial.print(ssid);
        delay(1000);
    }
    randomSeed(micros());
    Serial.println(""); Serial.print("WiFi connected with IP: "); Serial.println(WiFi.localIP());
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
        delay(0);
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
        for (int i = 0; i < numberOfSets; i++) {
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
        for (int i = 0; i < numberOfSets; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorGreenRed, colorGreenGreen, colorGreenBlue, start + startPos, end + startPos); // Red
            neopixelSet1.setRange(colorMenRed, colorMenGreen, colorMenBlue, startMenIndicator + startPos, endMenIndicator + startPos); // Red
        }
        redLight = false;
        greenLight = true;
        startSessionSwitchGreen = millis();
        mqttUtil.publishKeyValueInt(client, "redLight", 0);
        mqttUtil.publishKeyValueInt(client, "greenLight", 1);
    }
    delay(0);

    // Is it occupied or not?
    if (switchWomenStatus == 2) {
        if (VERBOSE) {
            Serial.println("Red light Women on!");
        }
        for (int i = 0; i < numberOfSets; i++) {
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
        for (int i = 0; i < numberOfSets; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorGreenRed, colorGreenGreen, colorGreenBlue, startWomen + startPos, endWomen + startPos); // Red
            neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos); // Red
        }
        redLightWomen = false;
        greenLightWomen = true;
        startWomenSessionSwitchGreen = millis();
        mqttUtil.publishKeyValueInt(client, "redLightWomen", 0);
        mqttUtil.publishKeyValueInt(client, "greenLightWomen", 1);
    }
    delay(0);

    // Are someone in line
    if (motionStatus == 2) {
        if (VERBOSE) {
            Serial.println("Yellow light on!");
        }
        neopixelSet1.setRange(colorYellowRed, colorYellowGreen, colorYellowBlue, startYellow, endYellow); // Yellow
        yellowLight = true;
        mqttUtil.publishKeyValueInt(client, "yellowLight", 1);

    } else if (motionStatus == 1) {
        if (VERBOSE) {
            Serial.println("Yellow light off!");
        }
        neopixelSet1.setRange(0, 0, 0, startYellow, endYellow);
        yellowLight = false;
        mqttUtil.publishKeyValueInt(client, "yellowLight", 0);

    }
}

void reconnectMqtt(uint32 ipAddress, long wifiDisconnectedPeriode) {
    long loopTimer = millis();
    while (!client.connected()) {
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        Serial.print("Attempting MQTT connection: ["); Serial.print(clientId); Serial.print("] : ");
        if (client.connect(clientId.c_str())) {
            Serial.println("Connected to MQTT!");
            client.subscribe(inTopic);
            mqttUtil.sendControllerInfo(client, ipAddress, wifiDisconnectedPeriode);
        } else {
            long now = millis();
            if (now - loopTimer > (MQTT_RESET_TIMER_SECONDS * 1000)) {
                ESP.restart();
            }
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
    for (int i = 0; i < numberOfSets; i++) {
        int startPos = (i * (endWomen + 1));
        neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos);
        neopixelSet1.setRange(colorMenRed, colorMenGreen, colorMenBlue, startMenIndicator + startPos, endMenIndicator + startPos);
    }
    setupWifi();
    TeliaWifi.connect(teliaWifiUser, teliaWifiPassword);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        setupWifi();
        TeliaWifi.connect(teliaWifiUser, teliaWifiPassword);
        return;
    }
    if (!client.connected()) {
        reconnectMqtt(WiFi.localIP(), wifiDisconnectedPeriode);
    }
    client.loop();
    long now = millis();

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (now - lastLoop1 > 10) {
        lastLoop1 = now;
        if (redLight) {
            if (now - startSessionSwitch > (1000 * 60 * 15)) { // > 30 min session
                menLightMode = 5;
            } else if (now - startSessionSwitch > (1000 * 60 * 10)) { // > 20 min session
                menLightMode = 4;
            } else if (now - startSessionSwitch > (1000 * 60 * 5)) { // > 10 min session
                menLightMode = 3;
            } else if (now - startSessionSwitch > (1000 * 60 * 3)) { // > 5 min session
                menLightMode = 2;
            } else {
                menLightMode = 1;
            }
        } else if (greenLight) {
            if (now - startSessionSwitchGreen > (1000 * 60 * 15)) { // > 30 min session
                menLightModeGreen = 5;
            } else if (now - startSessionSwitchGreen > (1000 * 60 * 10)) { // > 20 min session
                menLightModeGreen = 4;
            } else if (now - startSessionSwitchGreen > (1000 * 60 * 5)) { // > 10 min session
                menLightModeGreen = 3;
            } else if (now - startSessionSwitchGreen > (1000 * 60 * 3)) { // > 5 min session
                menLightModeGreen = 2;
            } else {
                menLightModeGreen = 1;
            }
        }
        if (redLightWomen) {
            if (now - startWomenSessionSwitch > (1000 * 60 * 15)) { // > 30 min session
                womenLightMode = 5;
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 10)) { // > 20 min session
                womenLightMode = 4;
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 5)) { // > 10 min session
                womenLightMode = 3;
            } else if (now - startWomenSessionSwitch > (1000 * 60 * 3)) { // > 5 min session
                womenLightMode = 2;
            } else {
                womenLightMode = 1;
            }
        } else if (greenLightWomen) {
            if (now - startWomenSessionSwitchGreen > (1000 * 60 * 15)) { // > 30 min session
                womenLightModeGreen = 5;
            } else if (now - startWomenSessionSwitchGreen > (1000 * 60 * 10)) { // > 20 min session
                womenLightModeGreen = 4;
            } else if (now - startWomenSessionSwitchGreen > (1000 * 60 * 5)) { // > 10 min session
                womenLightModeGreen = 3;
            } else if (now - startWomenSessionSwitchGreen > (1000 * 60 * 3)) { // > 5 min session
                womenLightModeGreen = 2;
            } else {
                womenLightModeGreen = 1;
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        if (redLight) {
            switch (menLightMode) {
                case 1:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.randomSparkRange(200, 200, 200, colorRedRed, colorRedGreen, colorRedBlue, 5, start + startPos, end + startPos, menIndex);
                    }
                    break;
                case 2:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, start + startPos, end + startPos, menIndex);
                    }
                    break;
                case 3:
                    if (now - menLoop > 40) {
                        menLoop = millis();
                        for (int i = 0; i < numberOfSets; i++) {
                            int startPos = (i * (endWomen + 1));
                            // neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, start + startPos, end + startPos, menIndex);
                            neopixelSet1.theaterChaseRange(colorRedRed, colorRedGreen, colorRedBlue, 1, start + startPos, end + startPos, menIndex);
                        }
                    }
                    break;
                case 4:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, start + startPos, end + startPos, menIndex);
                    }
                    break;
                case 5:
                    if (now - menLoop > 40) {
                        menLoop = millis();
                        for (int i = 0; i < numberOfSets; i++) {
                            int startPos = (i * (endWomen + 1));
                            neopixelSet1.strobeRange(colorRedRed, colorRedGreen, colorRedBlue, 5, 5, start + startPos, end + startPos, menIndex);
                        }
                    }
                    break;

            }
        } else if (greenLight) {
            switch (menLightModeGreen) {
                case 1:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.randomSparkRange(250, 250, 250, colorGreenRed, colorGreenGreen, colorGreenBlue, 5, start + startPos, end + startPos, menIndex);
                    }
                    break;
                case 2:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.randomSparkRange(250, 250, 250, colorGreenRed, colorGreenGreen, colorGreenBlue, 10, start + startPos, end + startPos, menIndex);
                    }
                    break;
                case 3:
                    if (now - menLoop > 100) {
                        menLoop = millis();
                        for (int i = 0; i < numberOfSets; i++) {
                            int startPos = (i * (endWomen + 1));
                            neopixelSet1.fadeInOutRange(colorGreenRed, colorGreenGreen, colorGreenBlue, 1, 100, start + startPos, end + startPos, menIndex);
                        }
                    }
                    break;
                case 4:
                    if (now - menLoop > 100) {
                        menLoop = millis();
                        for (int i = 0; i < numberOfSets; i++) {
                            int startPos = (i * (endWomen + 1));
                            neopixelSet1.theaterChaseRange(colorGreenRed, colorGreenGreen, colorGreenBlue, 1, start + startPos, end + startPos, menIndex);
                        }
                    }
                    break;
                case 5:
                    if (now - menLoop > 100) {
                        menLoop = millis();
                        for (int i = 0; i < numberOfSets; i++) {
                            int startPos = (i * (endWomen + 1));
                            neopixelSet1.theaterChaseRange(colorGreenRed, colorGreenGreen, colorGreenBlue, 1, start + startPos, end + startPos, menIndex);
                        }
                    }
                    break;
            }
        } else {
            if (now - menLoop > 100) {
                menLoop = millis();
                // No status.
                for (int i = 0; i < numberOfSets; i++) {
                    int startPos = (i * (endWomen + 1));
                    // neopixelSet1.randomSparkRange(200, 100, 100, 20, 20, 20, 10, startWomen + startPos, endWomen + startPos, womenIndex);
                    neopixelSet1.theaterChaseRange(50, 50, 50, 1, start + startPos, end + startPos, menIndex);
                    // neopixelSet1.theaterChaseRainbowRange(1, 10, 4, start + startPos, end + startPos, menIndex);
                }
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        if (redLightWomen) {
            switch (womenLightMode) {
                case 1:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.randomSparkRange(200, 200, 200, colorRedRed, colorRedGreen, colorRedBlue, 5, startWomen + startPos, endWomen + startPos, womenIndex);
                    }
                    break;
                case 2:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 1, 100, startWomen + startPos, endWomen + startPos, womenIndex);
                    }
                    break;
                case 3:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 10, 100, startWomen + startPos, endWomen + startPos, womenIndex);
                    }
                    break;
                case 4:
                    for (int i = 0; i < numberOfSets; i++) {
                        int startPos = (i * (endWomen + 1));
                        neopixelSet1.fadeInOutRange(colorRedRed, colorRedGreen, colorRedBlue, 20, 50, startWomen + startPos, endWomen + startPos, womenIndex);
                    }
                    break;
            }
        } else if (greenLightWomen) {
            for (int i = 0; i < numberOfSets; i++) {
                int startPos = (i * (endWomen + 1));
                neopixelSet1.randomSparkRange(200, 200, 200, colorGreenRed, colorGreenGreen, colorGreenBlue, 5, startWomen + startPos, endWomen + startPos, womenIndex);
            }
        } else {
            for (int i = 0; i < numberOfSets; i++) {
                int startPos = (i * (endWomen + 1));
                // neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos);
                neopixelSet1.randomSparkRange(200, 100, 100, 20, 20, 20, 10, startWomen + startPos, endWomen + startPos, womenIndex);
            }
        }
    }

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (now - lastLoop2 > 40) {
        lastLoop2 = now;
    }

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (now - lastLoop3 > 100) {
        lastLoop3 = now;
        if (yellowLight) {
            neopixelSet1.runningLightRange(colorYellowRed, colorYellowGreen, colorYellowBlue, startYellow, endYellow, 2, menIndex);
        } else {
            for (int i = 0; i < numberOfSets; i++) {
                int startPos = (i * (endWomen + 1));
                neopixelSet1.setRange(colorMenRed, colorMenGreen, colorMenBlue, startMenIndicator + startPos, endMenIndicator + startPos);
                // neopixelSet1.theaterChaseRange(colorMenRed, colorMenGreen, colorMenBlue, 0, startMenIndicator + startPos, endMenIndicator + startPos, menIndicatorIndex);
            }
        }
        for (int i = 0; i < numberOfSets; i++) {
            int startPos = (i * (endWomen + 1));
            neopixelSet1.setRange(colorWomenRed, colorWomenGreen, colorWomenBlue, startWomenIndicator + startPos, endWomenIndicator + startPos);
            // neopixelSet1.theaterChaseRange(colorWomenRed, colorWomenGreen, colorWomenBlue, 0, startWomenIndicator + startPos, endWomenIndicator + startPos, womenIndicatorIndex);
        }
    }

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (now - lastLoop4 > 5000) {
        lastLoop4 = now;
        Serial.println(".");

    }

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // Stuff to do at given time intervals.
    if (now - lastRun > (PUBLISH_INTERVAL * 1000)) {
        lastRun = now;

        if (DEEP_SLEEP) {
            Serial.print("Entering deep sleep mode for "); Serial.print(SLEEP_DELAY_IN_SECONDS); Serial.println(" seconds...");
            ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
        }
    }
    delay(10);
}
