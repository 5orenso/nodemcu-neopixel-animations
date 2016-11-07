// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define NUMPIXELS_1 20
#define PIN_1 D7
#define TOTAL_EFFECTS 11
#define DELAY_BETWEEN_EFFECTS 15000

Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);

#include <NeoPixelAnimations.h>
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);

 // Current animation effect
int  mode = 0;
 // Current color
int  r = 255,
     g = 145,
     b = 0;
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
    prevTime = millis();
}

void loop() {
    int t = 0;
    long now = millis();

    switch(mode) {
        case 0: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                // neopixelSet1.cylonBounce(r, g, b, 3);
                neopixelSet1.comet(r, g, b, 2, 8);
            }
            if (now - lastLoop2 > 10) {
                lastLoop2 = now;
                // neopixelSet1.bouncing(50, 50, 0, 4, 0);
                // neopixelSet4.theaterChase(r, g, b, 1);
                // neopixelSet4.randomSpark(255, 20, 147, 10, 10, 10, 20);

                // neopixelSet1.rainbowCycle(1, 3, 4);
                // neopixelSet2.rainbowCycle(0, 3, 10);
            }
            if (now - lastLoop3 > 40) {
                lastLoop3 = now;
                // neopixelSet1.cylonBounce(r, g, b, 4);
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
                // neopixelSet1.randomSpark(255, 20, 147, 10, 10, 10, 20);
                neopixelSet1.randomSpark(r, g, b, 10, 10, 10, 50);
            }
            break;
        }
        case 3: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.fadeInOut(r, g, b, 3);
            }
            break;
        }
        case 4: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.strobe(0, 0, 150, 5, 5);
            }
            break;
        }
        case 5: {
            if (now - lastLoop1 > 50) {
                lastLoop1 = now;
                neopixelSet1.cylonBounce(r, g, b, 4);
            }
            break;
        }
        case 6: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.theaterChase(r, g, b, 1);
            }
            break;
        }
        case 7: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.theaterChaseRainbow(1, 10, 4);
            }
            break;
        }
        case 8: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.rainbowCycle(1, 3, 4);
            }
            break;
        }
        case 9: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                // neopixelSet1.randomSpark(255, 20, 147, 10, 10, 10, 20);
                neopixelSet1.randomSpark(r, g, b, 10, 10, 10, 20);
                // neopixelSet1.fire(80, 80);
            }
            break;
        }
        case 10: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.bouncing(r, g, b, 2, 1);
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
