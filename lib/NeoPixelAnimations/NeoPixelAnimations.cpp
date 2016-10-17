#include <NeoPixelAnimations.h>
using namespace std;

NeoPixelAnimations::NeoPixelAnimations(Adafruit_NeoPixel &pixelsInput, int pixelCount) {
    pixels = pixelsInput;
    numPixels = pixelCount;
    // runningLight
    runningLightOffset = 0;
    // randomSpark
    randomSparkFlag = 0;
    randomSparkIndex = 0;
    // fadeInOut
    fadeInOutCounter = 0;
    fadeInOutDirection = 0;
    // strobe
    strobeCounter = 0;
    strobeOnFlag = 0;
    // cylonBounce
    cylonBounceOffset = 0;
    cylonBounceDirection = 0;
    // theaterChase
    theaterChaseOffset = 0;
    // theaterChaseRainbow
    theaterChaseRainbowOffset = 0;
    // rainbowCycle
    rainbowCycleWheelPos = 0;
    // fire
    heat[pixelCount];
    // bouncing
    bouncingInit = 0;
    bouncingStartHeight = 1;
    bouncingGravity = -9.81;
    bouncingimpactVelocityStart = sqrt(-2 * bouncingGravity * bouncingStartHeight);
}

/*
  Run light with 3 pixel in size.
    neopixelSet1.runningLight(r, g, b, 3)
*/
void NeoPixelAnimations::runningLight(int red, int green, int blue, int sizeOfSnake) {
    for (int i = 0; i < numPixels; i++) {
        int color_r = 0,
            color_g = 0,
            color_b = 0;
        if (i >= runningLightOffset && i < runningLightOffset + sizeOfSnake) {
            color_r = red;
            color_g = green;
            color_b = blue;
        }
        // Serial.print("i="); Serial.print(i);
        // Serial.print(", r="); Serial.print(color_r); Serial.print(", g="); Serial.print(color_g); Serial.print(", b="); Serial.println(color_b);
        pixels.setPixelColor(i, color_r, color_g, color_b);
    }
    pixels.show();
    runningLightOffset++;
    if (runningLightOffset > numPixels - sizeOfSnake) {
        runningLightOffset = 0;
    }
}

/*
  Spark on white background:
    neopixelSet1.randomSpark(r, g, b, 10, 10, 10)

  Spark on black background:
    neopixelSet1.randomSpark(r, g, b, 0, 0, 0)
*/
void NeoPixelAnimations::randomSpark(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue) {
    int color_r = red,
        color_g = green,
        color_b = blue;
    if (randomSparkFlag == 1) {
        randomSparkFlag = 0;
        color_r = bgRed;
        color_g = bgGreen;
        color_b = bgBlue;
    } else {
        randomSparkIndex = random(numPixels);
        randomSparkFlag = 1;
    }
    setAll(bgRed, bgGreen, bgBlue);
    pixels.setPixelColor(randomSparkIndex, color_r, color_g, color_b);
    pixels.show();
}

/*
  Set all pixels to a specific color.
    neopixelSet1.setAll(r, g, b)
*/
void NeoPixelAnimations::setAll(int red, int green, int blue) {
    // Serial.print("red="); Serial.print(red); Serial.print(", green="); Serial.print(green); Serial.print(", blue="); Serial.println(blue);
    for(int i = 0; i < numPixels; i++ ) {
        pixels.setPixelColor(i, red, green, blue);
    }
}

/*
    neopixelSet1.fadeInOut(r, g, b, 3);
*/
void NeoPixelAnimations::fadeInOut(int red, int green, int blue, int speed) {
    float r, g, b;
    r = (fadeInOutCounter / 256) * red;
    g = (fadeInOutCounter / 256) * green;
    b = (fadeInOutCounter / 256) * blue;
    setAll(round(r), round(g), round(b));
    pixels.show();

    if (fadeInOutDirection < 1) {
        fadeInOutCounter += speed;
    } else {
        fadeInOutCounter -= speed * 2;
    }
    if (fadeInOutCounter > 255) {
        fadeInOutDirection = 1;
    } else if (fadeInOutCounter <= 0) {
        fadeInOutCounter = 0;
        fadeInOutDirection = 0;
    }
}

/*
    neopixelSet3.strobe(r, g, b, 5, 5);
*/
void NeoPixelAnimations::strobe(int red, int green, int blue, int totalStrobes, int delayPeriods) {
    int r = red,
        g = green,
        b = blue;
    if (strobeOnFlag == 0) {
        strobeOnFlag = 1;
        r = 0;
        g = 0;
        b = 0;
    } else {
        strobeOnFlag = 0;
    }
    setAll(r, g, b);
    pixels.show();
    strobeCounter++;
    if (strobeCounter > (totalStrobes * 2)) {
        strobeOnFlag = 0;
        if (strobeCounter >= (totalStrobes * delayPeriods)) {
            strobeOnFlag = 1;
            strobeCounter = 0;
        }
    }
}

/*
    neopixelSet1.cylonBounce(r, g, b, 4);
*/
void NeoPixelAnimations::cylonBounce(int red, int green, int blue, int eyeSize) {
    setAll(0, 0, 0);
    pixels.setPixelColor(cylonBounceOffset, red/10, green/10, blue/10);
    for (int j = 1; j <= eyeSize; j++) {
        pixels.setPixelColor(cylonBounceOffset + j, red, green, blue);
    }
    pixels.setPixelColor(cylonBounceOffset + eyeSize, red/10, green/10, blue/10);
    pixels.show();
    if (cylonBounceDirection == 1) {
        cylonBounceOffset--;
    } else {
        cylonBounceOffset++;
    }
    if (cylonBounceOffset > numPixels - eyeSize - 2) {
        cylonBounceDirection = 1;
    } else if (cylonBounceOffset < 0 + 1) {
        cylonBounceDirection = 0;
    }
}

/*
    neopixelSet1.theaterChase(r, g, b, 1);
*/
void NeoPixelAnimations::theaterChase(int red, int green, int blue, int direction) {
    for(int i = 0; i < numPixels; i++) {
        pixels.setPixelColor(i,
            ((sin(i + theaterChaseOffset) * 127 + 128) / 255) * red,
            ((sin(i + theaterChaseOffset) * 127 + 128) / 255) * green,
            ((sin(i + theaterChaseOffset) * 127 + 128) / 255) * blue
        );
    }
    pixels.show();
    if (direction == 1) {
        theaterChaseOffset++;
        if (theaterChaseOffset > numPixels) {
            theaterChaseOffset = 0;
        }
    } else {
        theaterChaseOffset--;
        if (theaterChaseOffset < 0) {
            theaterChaseOffset = numPixels;
        }
    }
}

/*
    neopixelSet1.theaterChaseRainbow(1, 3, 10);
*/
void NeoPixelAnimations::theaterChaseRainbow(int direction, int speed, int brightness) {
    byte *c;
    for(int i = 0; i < numPixels; i++) {
        c = wheel(((i * 256 / numPixels) + theaterChaseRainbowWheelPos) & 255);
        pixels.setPixelColor(i,
            ((sin(i + theaterChaseRainbowOffset) * 127 + 128) / 255) * c[0]/brightness,
            ((sin(i + theaterChaseRainbowOffset) * 127 + 128) / 255) * c[1]/brightness,
            ((sin(i + theaterChaseRainbowOffset) * 127 + 128) / 255) * c[2]/brightness
        );
    }
    pixels.show();
    if (direction == 1) {
        theaterChaseRainbowOffset++;
        if (theaterChaseRainbowOffset > numPixels) {
            theaterChaseRainbowOffset = 0;
        }
        theaterChaseRainbowWheelPos += speed;
        if (theaterChaseRainbowWheelPos > 256 * 5) {
            theaterChaseRainbowWheelPos = 0;
        }
    } else {
        theaterChaseRainbowOffset--;
        if (theaterChaseRainbowOffset < 0) {
            theaterChaseRainbowOffset = numPixels;
        }
        theaterChaseRainbowWheelPos -= speed;
        if (theaterChaseRainbowWheelPos < 0) {
            theaterChaseRainbowWheelPos = 256 * 5;
        }
    }
}

/*
    byte *c;
    c = wheel(((i * 256 / numPixels) + theaterChaseRainbowWheelPos) & 255);

*/
byte* NeoPixelAnimations::wheel(int wheelPos) {
    static byte c[3];
    if (wheelPos < 85) {
        c[0] = wheelPos * 3;
        c[1] = 255 - wheelPos * 3;
        c[2] = 0;
    } else if(wheelPos < 170) {
        wheelPos -= 85;
        c[0]=255 - wheelPos * 3;
        c[1]=0;
        c[2]=wheelPos * 3;
    } else {
        wheelPos -= 170;
        c[0]=0;
        c[1]=wheelPos * 3;
        c[2]=255 - wheelPos * 3;
    }
    return c;
}

/*
  Cycle clockwise:
      neopixelSet1.rainbowCycle(1, 3, 10);
  Cycle counter clockwise:
      neopixelSet1.rainbowCycle(0, 3, 10);
*/
void NeoPixelAnimations::rainbowCycle(int direction, int speed, int brightness) {
    byte *c;
    for(int i = 0; i < numPixels; i++) {
        // c = wheel(rainbowCycleWheelPos + i);
        c = wheel(((i * 256 / numPixels) + rainbowCycleWheelPos) & 255);
        pixels.setPixelColor(i, c[0]/brightness, c[1]/brightness, c[2]/brightness);
    }
    pixels.show();
    if (direction == 1) {
        rainbowCycleWheelPos += speed;
        if (rainbowCycleWheelPos > 256 * 5) {
            rainbowCycleWheelPos = 0;
        }
    } else {
        rainbowCycleWheelPos -= speed;
        if (rainbowCycleWheelPos < 0) {
            rainbowCycleWheelPos = 256 * 5;
        }
    }
}

/*
  Random fire for a NeoPixel circle with 24 pixels
    neopixelSet1.fire(80, 80);
*/
void NeoPixelAnimations::fire(int cooling, int sparking) {
    int cooldown;
    // Step 1.  Cool down every cell a little
    for (int i = 0; i < numPixels; i++) {
        cooldown = random(0, ((cooling * 10) / numPixels) + 2);
        if(cooldown > heat[i]) {
            heat[i] = 0;
        } else {
            heat[i] = heat[i] - cooldown;
        }
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = numPixels - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < sparking) {
        int y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
    }
    // Step 4.  Convert heat to LED colors
    for (int j = 0; j < numPixels; j++) {
        setPixelHeatColor(j, heat[j]);
    }
    pixels.show();
}

/*
    setPixelHeatColor(j, heat[j]);
*/
void NeoPixelAnimations::setPixelHeatColor(int pixel, byte temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = round((temperature / 255.0) * 191);
    // calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
    // figure out which third of the spectrum we're in:
    if (t192 > 0x80) {                     // hottest
        pixels.setPixelColor(pixel, 255, 255, heatramp);
    } else if (t192 > 0x40) {             // middle
        pixels.setPixelColor(pixel, 255, heatramp, 0);
    } else {                               // coolest
        pixels.setPixelColor(pixel, heatramp, 0, 0);
    }
}

/*

*/
int NeoPixelAnimations::bouncing(int red, int green, int blue, int objectCount) {
    // float gravity = -9.81;
    // int startHeight = 1;
    // float height[objectCount];
    // float impactVelocityStart = sqrt(-2 * gravity * startHeight);
    // float impactVelocity[objectCount];
    // float timeSinceLastBounce[objectCount];
    // int   position[objectCount];
    // long  clockTimeSinceLastBounce[objectCount];
    // float dampening[objectCount];

    if (bouncingInit != 1) {
        bouncingPosition = new int[objectCount];
        bouncingHeight = new float[objectCount];
        bouncingImpactVelocity = new float[objectCount];
        bouncingTimeSinceLastBounce = new float[objectCount];
        bouncingDampening = new float[objectCount];
        bouncingClockTimeSinceLastBounce = new long[objectCount];

        for (int i = 0; i < objectCount; i++) {
            bouncingClockTimeSinceLastBounce[i] = millis();
            bouncingHeight[i] = bouncingStartHeight;
            bouncingPosition[i] = 0;
            bouncingImpactVelocity[i] = bouncingimpactVelocityStart;
            bouncingTimeSinceLastBounce[i] = 0;
            bouncingDampening[i] = 0.90 - float(i) / pow(objectCount, 2);
        }
        bouncingInit = 1;
    }
    for (int i = 0; i < objectCount; i++) {
        bouncingTimeSinceLastBounce[i] =  millis() - bouncingClockTimeSinceLastBounce[i];
        bouncingHeight[i] = 0.5 * bouncingGravity * pow(bouncingTimeSinceLastBounce[i] / 1000, 2.0) +
            bouncingImpactVelocity[i] * bouncingTimeSinceLastBounce[i] / 1000;
        if (bouncingHeight[i] < 0) {
            bouncingHeight[i] = 0;
            bouncingImpactVelocity[i] = bouncingDampening[i] * bouncingImpactVelocity[i];
            bouncingClockTimeSinceLastBounce[i] = millis();

            if (bouncingImpactVelocity[i] < 0.01) {
                bouncingImpactVelocity[i] = bouncingimpactVelocityStart;
            }
        }
        bouncingPosition[i] = round(bouncingHeight[i] * (numPixels - 1) / bouncingStartHeight);
    }

    setAll(0, 0, 0);
    for (int i = 0; i < objectCount; i++) {
        pixels.setPixelColor(bouncingPosition[i], red, green, blue);
    }
    pixels.show();
    return bouncingPosition[0];
}

