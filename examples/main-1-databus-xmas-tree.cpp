// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define NUMPIXELS_1 100
#define PIN_1 D7
#define TOTAL_EFFECTS 8
#define DELAY_BETWEEN_EFFECTS 15000

// Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_1, NEO_RGB + NEO_KHZ800);

#include <NeoPixelAnimations.h>
NeoPixelAnimations neopixelSet1 = NeoPixelAnimations(pixels1, NUMPIXELS_1);

 // Current animation effect
int  mode = 0;
 // Current color
int  r = 255,
     g = 255,
     b = 255;

// Candle 1900
int candle_r = 255,
    candle_g = 147,
    candle_b = 41;
// 40W Tungsten 2600
int tungsten_40w_r = 255,
    tungsten_40w_g = 197,
    tungsten_40w_b = 143;

//     100W Tungsten 2850
int tungsten_100w_r = 255,
    tungsten_100w_g = 214,
    tungsten_100w_b = 170;

// Halogen 3200
int halogen_r = 255,
    halogen_g = 241,
    halogen_b = 224;

// Carbon Arc 5200
int carbon_r = 255,
    carbon_g = 250,
    carbon_b = 244;

// High Noon Sun 5400
int high_noon_r = 255,
    high_noon_g = 255,
    high_noon_b = 251;

// Direct Sunlight 6000
int direct_sun_r = 255,
    direct_sun_g = 255,
    direct_sun_b = 255;

// Overcast Sky 7000
int overcast_sky_r = 201,
    overcast_sky_g = 226,
    overcast_sky_b = 255;

// Clear Blue Sky 20000
int clear_blue_sky_r = 64,
    clear_blue_sky_g = 156,
    clear_blue_sky_b = 255;


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
            if (now - lastLoop1 > 50) {
                lastLoop1 = now;
                // neopixelSet1.cylonBounce(r, g, b, 3);
                neopixelSet1.comet(r, g, b, 2, 10);
            }
            if (now - lastLoop2 > 10) {
                lastLoop2 = now;
                // neopixelSet1.bouncing(50, 50, 0, 4, 0);
                // neopixelSet4.theaterChase(r, g, b, 1);
                // neopixelSet4.randomSpark(255, 20, 147, 10, 10, 10, 20);
                float factor = .8;
                // neopixelSet1.randomSpark(r, g, b, (int)(candle_r * factor), (int)(candle_g * factor), (int)(candle_b * factor), 5);
                // neopixelSet1.randomSpark(r, g, b, 5, 5, 5, 10);

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
            if (now - lastLoop2 > 10) {
                lastLoop2 = now;
                float factor = .7;
                neopixelSet1.randomSpark(r, g, b, (int)(candle_r * factor), (int)(candle_g * factor), (int)(candle_b * factor), 5);
            }
            break;
        }
        // ---------------------------------------------------------------------
        case 2: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                float factor = .5;
                neopixelSet1.randomSpark(r, g, b, (int)(candle_r * factor), (int)(candle_g * factor), (int)(candle_b * factor), 5);
            }
            break;
        }
        case 3: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                float factor = .3;
                neopixelSet1.randomSpark(r, g, b, (int)(candle_r * factor), (int)(candle_g * factor), (int)(candle_b * factor), 5);
            }
            break;
        }
        case 4: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                float factor = .5;
                neopixelSet1.randomSpark(r, g, b, (int)(tungsten_40w_r * factor), (int)(tungsten_40w_g * factor), (int)(tungsten_40w_b * factor), 5);
            }
            break;
        }
        case 5: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                float factor = .5;
                neopixelSet1.randomSpark(r, g, b, (int)(halogen_r * factor), (int)(halogen_g * factor), (int)(halogen_b * factor), 5);
            }
            break;
        }
        case 6: {
            if (now - lastLoop1 > 10) {
                lastLoop1 = now;
                float factor = .5;
                neopixelSet1.randomSpark(r, g, b, (int)(clear_blue_sky_r * factor), (int)(clear_blue_sky_g * factor), (int)(clear_blue_sky_b * factor), 5);
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
