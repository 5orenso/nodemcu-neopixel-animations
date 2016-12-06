#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NeoPixelAnimations.h>

const char* PACKAGE_NAME = "main-2-databus-toilet-light";

#define DEBUG false
#define VERBOSE true
#define PUBLISH_INTERVAL 30
#define SLEEP_DELAY_IN_SECONDS  30
#define PIN_MEN D7
#define PIN_WOMEN D6
#define NUMPIXELS_MEN 14
#define NUMPIXELS_WOMEN 14

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const int   mqtt_port = MQTT_PORT;
const char* outTopic = MQTT_OUT_TOPIC;
const char* inTopic = MQTT_IN_TOPIC;

WiFiClient espClient;
PubSubClient client(espClient);
long lastRun = millis();
int nodemcuChipId = ESP.getChipId(); // returns the ESP8266 chip ID as a 32-bit integer.

float factor = .6;

Adafruit_NeoPixel pixelsMen = Adafruit_NeoPixel(NUMPIXELS_MEN, PIN_MEN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsWomen = Adafruit_NeoPixel(NUMPIXELS_WOMEN, PIN_WOMEN, NEO_GRB + NEO_KHZ800);

NeoPixelAnimations neopixelSetMen = NeoPixelAnimations(pixelsMen, NUMPIXELS_MEN);
NeoPixelAnimations neopixelSetWomen = NeoPixelAnimations(pixelsWomen, NUMPIXELS_WOMEN);

void setupWifi() {
    delay(10);
    WiFi.disconnect();
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

    // OTA start
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    // OTA end

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
    StaticJsonBuffer<100> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject((char*)payload);
    if (!root.success()) {
        if (VERBOSE) {
            Serial.println("handleUpdate: payload parse FAILED");
        }
        return;
    }
    int thisChipId = root["chipId"];
    int switchStatusMen = root["switch"];
    int switchStatusWomen = root["switchWomen"];
    int motionStatus = root["motion"];
    if (DEBUG) {
        Serial.println("handleUpdate payload:"); root.prettyPrintTo(Serial); Serial.println();
        Serial.print("chipId: "); Serial.print(nodemcuChipId);
        Serial.print(" switchWomen: "); Serial.print(switchStatusWomen);
        Serial.print(" switch: "); Serial.println(switchStatusMen);
    }

    if (switchStatusMen == 2) { // Is mens room occupied or not?
        if (VERBOSE) {
            Serial.println("Mens room: Red light on!");
        }
        neopixelSetMen.setRange((int)(255 * factor), (int)(0 * factor), (int)(0 * factor), 0, 13); // Red
    } else if (switchStatusMen == 1) {
        if (VERBOSE) {
            Serial.println("Mens room: Green light on!");
        }
        neopixelSetMen.setRange((int)(0 * factor), (int)(255 * factor), (int)(0 * factor), 0, 13); // Red
    } else if (switchStatusWomen == 2) {  // Is ladies room occupied or not?
        if (VERBOSE) {
            Serial.println("Ladies room: Red light on!");
        }
        neopixelSetWomen.setRange((int)(255 * factor), (int)(0 * factor), (int)(0 * factor), 0, 13); // Red
    } else if (switchStatusWomen == 1) {
        if (VERBOSE) {
            Serial.println("Ladies room: Green light on!");
        }
        neopixelSetWomen.setRange((int)(0 * factor), (int)(255 * factor), (int)(0 * factor), 0, 13); // Red
    }
}

void sendControllerInfo() {
    if (client.connected()) {
        // --[ Publish this device to AWS IoT ]----------------------------------------
        // String nodemcuResetReason = ESP.getResetReason(); // returns String containing the last reset resaon in human readable format.
        int nodemcuFreeHeapSize = ESP.getFreeHeap(); // returns the free heap size.
        // Several APIs may be used to get flash chip info:
        int nodemcuFlashChipId = ESP.getFlashChipId(); // returns the flash chip ID as a 32-bit integer.
        int nodemcuFlashChipSize = ESP.getFlashChipSize(); // returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
        // int nodemcuFlashChipSpeed = ESP.getFlashChipSpeed(void); // returns the flash chip frequency, in Hz.
        // int nodemcuCycleCount = ESP.getCycleCount(); // returns the cpu instruction cycle count since start as an unsigned 32-bit. This is useful for accurate timing of very short actions like bit banging.
        // WiFi.macAddress(mac) is for STA, WiFi.softAPmacAddress(mac) is for AP.
        // int nodemcuIP; // = WiFi.localIP(); // is for STA, WiFi.softAPIP() is for AP.
        char msg[150];
        uint32 ipAddress;
        char ipAddressFinal[16];
        ipAddress = WiFi.localIP();
        if (ipAddress) {
            const int NBYTES = 4;
            uint8 octet[NBYTES];
            for(int i = 0 ; i < NBYTES ; i++) {
                octet[i] = ipAddress >> (i * 8);
            }
            sprintf(ipAddressFinal, "%d.%d.%d.%d", octet[0], octet[1], octet[2], octet[3]);
        }
        snprintf(msg, 150, "{ \"chipId\": %d, \"freeHeap\": %d, \"ip\": \"%s\", \"ssid\": \"%s\" }",
            nodemcuChipId, nodemcuFreeHeapSize, ipAddressFinal, ssid
        );
        if (VERBOSE) {
            Serial.print("Publish message: "); Serial.println(msg);
        }
        client.publish(outTopic, msg);

        // More info about the software.
        snprintf(msg, 150, "{ \"chipId\": %d, \"ip\": \"%s\", \"sw\": \"%s\" }",
            nodemcuChipId, ipAddressFinal, PACKAGE_NAME
        );
        if (VERBOSE) {
            Serial.print("Publish message: "); Serial.println(msg);
        }
        client.publish(outTopic, msg);
    }
}

void reconnectMqtt() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(inTopic);
            sendControllerInfo();
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_MEN, OUTPUT);
    pinMode(PIN_WOMEN, OUTPUT);
	pixelsMen.begin();
	pixelsWomen.begin();
    neopixelSetMen.setAll(0, 0, 0);
    neopixelSetWomen.setAll(0, 0, 0);
    setupWifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnectMqtt();
    }
    client.loop();

    // Listen for OTA requests
    ArduinoOTA.handle();

    long now = millis();

    // Stuff to do at given time intervals.
    if (now - lastRun > (PUBLISH_INTERVAL * 1000)) {
        lastRun = now;

    }
}
