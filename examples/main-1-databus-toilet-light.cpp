#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DEBUG false
#define VERBOSE true
#define DEEP_SLEEP false

#define LISTEN_TO_CHIP 1607191

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

WiFiClient espClient;
PubSubClient client(espClient);
long lastRun = millis();
char msg[150];
int chipId = ESP.getChipId();


Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);

#include <NeoPixelAnimations.h>
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);

 // Current animation effect
int  mode = 0;

// Timers
int prevTime,
    lastLoop1,
    lastLoop2,
    lastLoop3;

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
    // const char* thisChipId = root["chipId"];
    int thisChipId = root["chipId"];
    int switchStatus = root["switch"];
    int motionStatus = root["motion"];
    if (DEBUG) {
        Serial.println("handleUpdate payload:"); root.prettyPrintTo(Serial); Serial.println();
        Serial.print("chipId: "); Serial.print(chipId); Serial.print(" switch: "); Serial.println(switchStatus);
    }

    if (LISTEN_TO_CHIP == thisChipId) {
        // Is it occupied or not?
        if (switchStatus == 2) {
            if (VERBOSE) {
                Serial.println("Red light on!");
            }
            digitalWrite(PIN_GREEN_LED, 0);
            digitalWrite(PIN_RED_LED, 1);
            // neopixelSet1.setRange(20, 20, 0, 0, 18); // Yellow
            // neopixelSet1.setRange(0, 0, 0, 18, 36); // Green off
            // neopixelSet1.setRange(80, 0, 0, 36, 54); // Red
            float factor = .1;
            neopixelSet1.setRange((int)(227 * factor), (int)(46 * factor), (int)(54 * factor), 45, 54); // Telia Purple
            neopixelSet1.setRange(60, 0, 0, 9, 45); // Red
        } else if (switchStatus == 1) {
            if (VERBOSE) {
                Serial.println("Green light on!");
            }
            digitalWrite(PIN_GREEN_LED, 1);
            digitalWrite(PIN_RED_LED, 0);
            // neopixelSet1.setRange(20, 20, 0, 0, 18); // Yellow
            // neopixelSet1.setRange(0, 80, 0, 18, 36); // Green
            // neopixelSet1.setRange(0, 0, 0, 36, 54); // Red off
            // #9A0BE3 154	11	227
            float factor = .1;
            neopixelSet1.setRange((int)(227 * factor), (int)(46 * factor), (int)(54 * factor), 45, 54); // Telia Purple
            neopixelSet1.setRange(0, 60, 0, 9, 45); // Green
        }

        // Are someone in line
        if (motionStatus == 2) {
            if (VERBOSE) {
                Serial.println("Yellow light on!");
            }
            digitalWrite(PIN_YELLOW_LED, 1);
            float factor = .2;
            neopixelSet1.setRange((int)(255 * factor), (int)(165 * factor), (int)(0 * factor), 0, 9); // Yellow
        } else if (motionStatus == 1) {
            if (VERBOSE) {
                Serial.println("Yellow light off!");
            }
            digitalWrite(PIN_YELLOW_LED, 0);
            neopixelSet1.setRange(0, 0, 0, 0, 9); // Yellow
        }

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
    pinMode(PIN_GREEN_LED, OUTPUT);
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
	pixels1.begin();
    prevTime = millis();
    neopixelSet1.setAll(0, 0, 0);
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
        // if (DEBUG) {
        //     Serial.print(", Light: "); Serial.print(valueLightChar);
        // }

        if (DEEP_SLEEP) {
            Serial.print("Entering deep sleep mode for "); Serial.print(SLEEP_DELAY_IN_SECONDS); Serial.println(" seconds...");
            ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
        }
    }
}
