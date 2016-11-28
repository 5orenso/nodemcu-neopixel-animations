#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NeoPixelAnimations.h>

#define DEBUG false
#define VERBOSE true
#define PUBLISH_INTERVAL 30
#define SLEEP_DELAY_IN_SECONDS  30
#define PIN_MEN D7
#define PIN_WOMEN D6
#define NUMPIXELS_MEN 14
#define NUMPIXELS_WOMEN 14
#define LISTEN_TO_CHIP 1607191

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const int   mqtt_port = MQTT_PORT;
const char* outTopic = MQTT_OUT_TOPIC;
const char* inTopic = MQTT_IN_TOPIC;

WiFiClient espClient;
PubSubClient client(espClient);
long lastRun = millis();
int chipId = ESP.getChipId();

float factor = .6;

Adafruit_NeoPixel pixelsMen = Adafruit_NeoPixel(NUMPIXELS_MEN, PIN_MEN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsWomen = Adafruit_NeoPixel(NUMPIXELS_WOMEN, PIN_WOMEN, NEO_GRB + NEO_KHZ800);

NeoPixelAnimations neopixelSetMen = NeoPixelAnimations(pixelsMen, NUMPIXELS_MEN);
NeoPixelAnimations neopixelSetWomen = NeoPixelAnimations(pixelsWomen, NUMPIXELS_WOMEN);

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
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
        Serial.print("chipId: "); Serial.print(chipId);
        Serial.print(" switchWomen: "); Serial.print(switchStatusWomen);
        Serial.print(" switch: "); Serial.println(switchStatusMen);
    }

    if (LISTEN_TO_CHIP == thisChipId) {
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

        // // Are someone in line
        // if (motionStatus == 2) {
        //     if (VERBOSE) {
        //         Serial.println("Yellow light on!");
        //     }
        //     float factor = .2;
        //     neopixelSetMen.setRange((int)(255 * factor), (int)(165 * factor), (int)(0 * factor), 0, 9); // Yellow
        // } else if (motionStatus == 1) {
        //     if (VERBOSE) {
        //         Serial.println("Yellow light off!");
        //     }
        //     neopixelSetMen.setRange(0, 0, 0, 0, 9); // Yellow
        // }

    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // client.publish(outTopic, "{ \"chipId\": chipId, \"ping\": \"hello world\" }");
            // ... and resubscribe
            client.subscribe(inTopic);
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
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
	pixelsMen.begin();
	pixelsWomen.begin();
    neopixelSetMen.setAll(0, 0, 0);
    neopixelSetWomen.setAll(0, 0, 0);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    long now = millis();

    // Stuff to do at given time intervals.
    if (now - lastRun > (PUBLISH_INTERVAL * 1000)) {
        lastRun = now;

    }
}
