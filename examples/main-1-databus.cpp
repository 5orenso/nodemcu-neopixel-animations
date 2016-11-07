// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define NUMPIXELS_1 24
#define PIN_1 D5
#define TOTAL_EFFECTS 10
#define DELAY_BETWEEN_EFFECTS 10000

Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);

#include <NeoPixelAnimations.h>
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);

 // Current animation effect
int  mode = 0;
 // Current color
int  r = 100,
     g = 100,
     b = 100;
// Timers
int prevTime,
    lastLoop1;

void setup() {
    Serial.begin(115200);
	pixels1.begin();
    prevTime = millis();
}

void loop() {
    int t = 0;
    long now = millis();

    switch(mode) {
        case 0: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.bouncing(255, 20, 147, 4, 0);
            }
            break;
        }
        case 1: {
            if (now - lastLoop1 > 50) {
                lastLoop1 = now;
                neopixelSet1.runningLight(r, g, b, 3);
            }
            break;
        }
        // ---------------------------------------------------------------------
        case 2: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.randomSpark(r, g, b, 10, 10, 10, 20);
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
                neopixelSet1.strobe(0, 0, 200, 5, 5);
            }
            break;
        }
        case 5: {
            if (now - lastLoop1 > 20) {
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
                neopixelSet1.theaterChaseRainbow(1, 10, 10);
            }
            break;
        }
        case 8: {
            if (now - lastLoop1 > 50) {
                lastLoop1 = now;
                neopixelSet1.rainbowCycle(1, 3, 10);
            }
            break;
        }
        case 9: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.fire(50, 120);
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
