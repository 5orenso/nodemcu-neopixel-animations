// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define NUMPIXELS_1 24
#define NUMPIXELS_2 12
#define NUMPIXELS_3 4
#define NUMPIXELS_4 60
#define PIN_1 D5
#define PIN_2 D6
#define PIN_3 D7
#define TOTAL_EFFECTS 10
#define DELAY_BETWEEN_EFFECTS 10000

Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS_2, PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(NUMPIXELS_3, PIN_3, NEO_GRB + NEO_KHZ800);

#include <NeoPixelAnimations.h>
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);
NeoPixelAnimations neopixelSet2 = NeoPixelAnimations(pixels2, NUMPIXELS_2);
NeoPixelAnimations neopixelSet3 = NeoPixelAnimations(pixels3, NUMPIXELS_3);

 // Current animation effect
int  mode = 0;
 // Current color
int  r = 100,
     g = 100,
     b = 100;
// Timers
int prevTime,
    lastLoop1,
    lastLoop2,
    lastLoop3;

void setup() {
    Serial.begin(115200);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
	pixels1.begin();
	pixels2.begin();
	pixels3.begin();
    prevTime = millis();
}

void loop() {
    int t = 0;
    long now = millis();

    switch(mode) {
        case 0: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                // Serial.print(neopixelSet1.bouncing(50, 50, 0, 2, 0)); Serial.println();
                // neopixelSet1.theaterChaseRainbow(1, round(255/NUMPIXELS_1), 10);
                neopixelSet2.theaterChaseRainbow(1, round(255/NUMPIXELS_2), 4);
                // neopixelSet2.theaterChase(r, g, b, 0);
                // Serial.print("case 0: calling neopixelSet1.run(r, g, b) : "); Serial.println(i);
            }
            if (now - lastLoop2 > 10) {
                lastLoop2 = now;
                neopixelSet1.bouncing(50, 50, 0, 4, 0);
                // neopixelSet4.theaterChase(r, g, b, 1);
                // neopixelSet4.randomSpark(255, 20, 147, 10, 10, 10, 20);

                // Serial.print(neopixelSet1.bouncing(50, 50, 0, 2, 0)); Serial.println();
                // neopixelSet1.rainbowCycle(1, 3, 10);
                // neopixelSet2.rainbowCycle(0, 3, 10);
                neopixelSet3.rainbowCycle(0, 3, 4);
            }
            if (now - lastLoop3 > 40) {
                lastLoop3 = now;
                // neopixelSet4.strobe(0, 0, 200, 5, 5);
                // neopixelSet1.fire(80, 80);
                // neopixelSet1.theaterChase(r, g, b, 1);
                // neopixelSet3.strobe(0, 0, 20, 5, 5);
            }
            break;
        }
        case 1: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.runningLight(r, g, b, 3);
                neopixelSet2.runningLight(r, g, b, 2);
                neopixelSet3.runningLight(r, g, b, 1);
            }
            if (now - lastLoop2 > 20) {
                lastLoop2 = now;
                // neopixelSet4.fire(50, 120);
            }
            break;
        }
        // ---------------------------------------------------------------------
        case 2: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.randomSpark(r, g, b, 10, 10, 10, 20);
                neopixelSet2.randomSpark(r, g, b, 0, 0, 0, 20);
                neopixelSet3.randomSpark(r, g, b, 10, 10, 10, 20);
                // neopixelSet4.randomSpark(255, 20, 147, 10, 10, 10, 20);
            }
            break;
        }
        case 3: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.fadeInOut(r, g, b, 3);
                neopixelSet2.fadeInOut(r, g, b, 3);
                neopixelSet3.fadeInOut(r, g, b, 3);
                // neopixelSet4.fadeInOut(r, g, b, 3);
            }
            break;
        }
        case 4: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.strobe(40, 0, 0, 5, 5);
                neopixelSet2.strobe(0, 40, 40, 5, 5);
                neopixelSet3.strobe(0, 0, 100, 5, 5);
                // neopixelSet4.strobe(0, 0, 140, 5, 5);
            }
            break;
        }
        case 5: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.cylonBounce(r, g, b, 4);
                neopixelSet2.cylonBounce(r, g, b, 2);
                neopixelSet3.cylonBounce(r, g, b, 2);
                // neopixelSet4.cylonBounce(r, g, b, 1);
            }
            break;
        }
        case 6: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.theaterChase(r, g, b, 1);
                neopixelSet2.theaterChase(r, g, b, 0);
                neopixelSet3.theaterChase(r, g, b, 1);
                // neopixelSet4.theaterChase(r, g, b, 1);
            }
            break;
        }
        case 7: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.theaterChaseRainbow(1, 10, 4);
                neopixelSet2.theaterChaseRainbow(0, 10, 4);
                neopixelSet3.theaterChaseRainbow(1, 10, 4);
                // neopixelSet4.theaterChaseRainbow(1, 10, 10);
            }
            break;
        }
        case 8: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.rainbowCycle(1, 3, 4);
                neopixelSet2.rainbowCycle(0, 3, 4);
                neopixelSet3.rainbowCycle(1, 3, 4);
                // neopixelSet4.rainbowCycle(1, 3, 10);
            }
            break;
        }
        case 9: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.fire(80, 80);
                neopixelSet2.fire(80, 80);
                neopixelSet3.fire(80, 80);
                // neopixelSet4.fire(80, 80);
            }
            break;
        }
        case 10: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.bouncing(255, 20, 147, 2, 0);
                neopixelSet2.bouncing(255, 20, 147, 1, 0);
                neopixelSet3.bouncing(255, 20, 147, 1, 0);
                // neopixelSet4.bouncing(255, 20, 147, 4, 0);
            }
            break;
        }

    }

    t = millis();
    if ((t - prevTime) > DELAY_BETWEEN_EFFECTS) {
        mode++;
        if (mode >= TOTAL_EFFECTS) {
            mode = 0;
        }
        Serial.print("mode = "); Serial.println(mode);
        prevTime = t;
    }
}
