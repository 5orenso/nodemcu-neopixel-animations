// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>       /* round, floor, ceil, trunc */

// #define D0 16
// #define D1 5 // I2C Bus SCL (clock)
// #define D2 4 // I2C Bus SDA (data)
// #define D3 0
// #define D4 2 // Same as "LED_BUILTIN", but inverted logic
// #define D5 14 // SPI Bus SCK (clock)
// #define D6 12 // SPI Bus MISO
// #define D7 13 // SPI Bus MOSI
// #define D8 15 // SPI Bus SS (CS)
// #define D9 3 // RX0 (Serial console)
// #define D10 1 // TX0 (Serial console)

#define NUMPIXELS_1 24
#define NUMPIXELS_2 12
#define NUMPIXELS_3 4
#define PIN_1 D5
#define PIN_2 D6
#define PIN_3 D7
#define TOTAL_EFFECTS 1
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
int  r = 50,
     g = 50,
     b = 50;
// Timers
int prevTime,
    lastLoop1,
    lastLoop2,
    lastLoop3;

void setup() {
    Serial.begin(115200);
    pinMode(D7, OUTPUT);
	pixels1.begin();
	pixels2.begin();
    prevTime = millis();
}

void loop() {
    int t = 0;
    long now = millis();

    switch(mode) {
        case 0: { // Random sparks - just one LED on at a time!
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                // Serial.print(neopixelSet1.bouncing(50, 50, 0, 2)); Serial.println();
                // neopixelSet1.theaterChaseRainbow(1, round(255/NUMPIXELS_1), 10);
                neopixelSet2.theaterChaseRainbow(1, round(255/NUMPIXELS_2), 10);
                // neopixelSet2.theaterChase(r, g, b, 0);
                // Serial.print("case 0: calling neopixelSet1.run(r, g, b) : "); Serial.println(i);
            }
            if (now - lastLoop2 > 10) {
                lastLoop2 = now;
                neopixelSet1.bouncing(50, 50, 0, 4);
                // Serial.print(neopixelSet1.bouncing(50, 50, 0, 2)); Serial.println();
                // neopixelSet1.rainbowCycle(1, 3, 10);
                // neopixelSet2.rainbowCycle(0, 3, 10);
                neopixelSet3.rainbowCycle(0, 3, 10);
            }
            if (now - lastLoop3 > 60) {
                lastLoop3 = now;
                // neopixelSet1.fire(80, 80);
                // neopixelSet1.theaterChase(r, g, b, 1);
                // neopixelSet3.strobe(0, 0, 20, 5, 5);
            }
            break;
        }
        case 1: { // Running light
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.runningLight(r, g, b, 3);
                neopixelSet2.runningLight(r, g, b, 2);
                neopixelSet3.runningLight(r, g, b, 1);
            }
            // if (now - lastLoop2 > 20) {
            //     lastLoop2 = now;
            //     int red   = random(50),
            //         green = random(50),
            //         blue  = random(50);
            //     neopixelSet3.randomSpark(red, green, blue, 0, 0, 0);
            // }
            break;
        }
        // ---------------------------------------------------------------------
        case 2: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.randomSpark(r, g, b, 10, 10, 10);
                neopixelSet2.randomSpark(r, g, b, 0, 0, 0);
                neopixelSet3.randomSpark(r, g, b, 10, 10, 10);
            }
        }
        case 3: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.fadeInOut(r, g, b, 3);
                neopixelSet2.fadeInOut(r, g, b, 3);
                neopixelSet3.fadeInOut(r, g, b, 3);
            }
            break;
        }
        case 4: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.strobe(40, 0, 0, 5, 5);
                neopixelSet2.strobe(0, 40, 40, 5, 5);
                neopixelSet3.strobe(0, 0, 40, 5, 5);
            }
            break;
        }
        case 5: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.cylonBounce(r, g, b, 4);
                neopixelSet2.cylonBounce(r, g, b, 2);
                neopixelSet3.cylonBounce(r, g, b, 1);
            }
            break;
        }
        case 6: {
            if (now - lastLoop1 > 40) {
                lastLoop1 = now;
                neopixelSet1.theaterChase(r, g, b, 1);
                neopixelSet2.theaterChase(r, g, b, 0);
                neopixelSet3.theaterChase(r, g, b, 1);
            }
        }
        case 7: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
                neopixelSet1.theaterChaseRainbow(1, 10, 10);
                neopixelSet2.theaterChaseRainbow(0, 10, 10);
                neopixelSet3.theaterChaseRainbow(1, 10, 10);
            }
        }
        case 8: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                neopixelSet1.rainbowCycle(1, 3, 10);
                neopixelSet2.rainbowCycle(0, 3, 10);
                neopixelSet3.rainbowCycle(1, 3, 10);
            }
        }
        case 9: {
            if (now - lastLoop1 > 100) {
                lastLoop1 = now;
            }
        }

    }

    t = millis();
    if ((t - prevTime) > DELAY_BETWEEN_EFFECTS) {      // Every 8 seconds...
        mode++;                        // Next mode
        if (mode >= TOTAL_EFFECTS) {                 // End of modes?
            mode = 0;                    // Start modes over
            //     color >>= 8;                 // Next color R->G->B
            //     if(!color) color = 0xFF0000; // Reset to red
        }
        Serial.print("mode = "); Serial.println(mode);
        prevTime = t;
    }
}
