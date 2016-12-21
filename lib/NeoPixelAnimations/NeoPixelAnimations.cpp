#include <NeoPixelAnimations.h>
using namespace std;

NeoPixelAnimations::NeoPixelAnimations(Adafruit_NeoPixel &pixelsInput, int pixelCount) {
    pixels = pixelsInput;
    numPixels = pixelCount;
    // runningLightRange
    runningLightRangeOffsets = new int[10];
    // randomSparkRange
    randomSparkRangeFlags = new int[10];
    randomSparkRangeIndexs = new int[10];
    // fadeInOutRange
    fadeInOutRangeCounters = new float[10];
    fadeInOutRangeDirections = new int[10];
    // strobeRange
    strobeRangeCounters = new int[10];
    strobeOnRangeFlags = new int[10];
    // cylonBounce
    cylonBounceOffset = new int[10];
    cylonBounceDirection = new int[10];
    // comet
    cometOffset = new int[10];
    cometDirection = new int[10];
    // theaterChase
    theaterChaseRangeOffset = new int[10];
    // theaterChaseRainbow
    theaterChaseRainbowOffset = new int[10];
    theaterChaseRainbowWheelPos = new int[10];
    // rainbowCycle
    rainbowCycleWheelPos = new int[10];
    // fire
    fireInit = new int[10];
    // heat[pixelCount];
    // bouncing
    bouncingInit = 0;
    bouncingStartHeight = 1;
    bouncingGravity = -9.81;
    bouncingimpactVelocityStart = sqrt(-2 * bouncingGravity * bouncingStartHeight);
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
    pixels.show();

}

/*
  Set range of pixels to a specific color.
    neopixelSet1.setRange(r, g, b, start, end)
*/
void NeoPixelAnimations::setRange(int red, int green, int blue, int start, int end) {
    // Serial.print("red="); Serial.print(red); Serial.print(", green="); Serial.print(green); Serial.print(", blue="); Serial.println(blue);
    for(int i = start; i <= end; i++ ) {
        pixels.setPixelColor(i, red, green, blue);
    }
    pixels.show();

}

/*
  Run light with 3 pixel in size.
    neopixelSet1.runningLight(r, g, b, 3)
    neopixelSet1.runningLightRange(red, green, blue, start, end, 3);
    neopixelSet1.runningLightRange(red, green, blue, start, end, 3, index);
*/
void NeoPixelAnimations::runningLight(int red, int green, int blue, int sizeOfSnake) {
    int index = 0;
    runningLightRange(red, green, blue, 0, numPixels, sizeOfSnake, index);
}
void NeoPixelAnimations::runningLightRange(int red, int green, int blue, int start, int end, int sizeOfSnake) {
    int index = 0;
    runningLightRange(red, green, blue, start, end, sizeOfSnake, index);
}
void NeoPixelAnimations::runningLightRange(int red, int green, int blue, int start, int end, int sizeOfSnake, int index) {
    for (int i = start; i <= end; i++) {
        int color_r = 0,
            color_g = 0,
            color_b = 0;
        if (i >= runningLightRangeOffsets[index] && i < runningLightRangeOffsets[index] + sizeOfSnake) {
            color_r = red;
            color_g = green;
            color_b = blue;
        }
        pixels.setPixelColor(i, color_r, color_g, color_b);
    }
    pixels.show();
    runningLightRangeOffsets[index]++;
    if (runningLightRangeOffsets[index] > end - sizeOfSnake) {
        runningLightRangeOffsets[index] = start;
    } else if (runningLightRangeOffsets[index] < start) {
        runningLightRangeOffsets[index] = end;
    }
}

/*
  Spark on white background:
    neopixelSet1.randomSpark(r, g, b, 10, 10, 10)

  Spark on black background:
    neopixelSet1.randomSpark(r, g, b, 0, 0, 0)

  Spark on a specific color and range:
    neopixelSet1.randomSparkRange(200, 200, 200, red, green, blue, 5, start, end);
    neopixelSet1.randomSparkRange(200, 200, 200, red, green, blue, 5, start, end, index);
*/
void NeoPixelAnimations::randomSpark(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking) {
    int index = 0;
    randomSparkRange(red, green, blue, bgRed, bgGreen, bgBlue, sparking, 0, numPixels, index);
}
void NeoPixelAnimations::randomSparkRange(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking, int start, int end) {
    int index = 0;
    randomSparkRange(red, green, blue, bgRed, bgGreen, bgBlue, sparking, start, end, index);
}
void NeoPixelAnimations::randomSparkRange(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking, int start, int end, int index) {
    int color_r = red,
        color_g = green,
        color_b = blue;
    if (randomSparkRangeFlags[index] == 1) {
        randomSparkRangeFlags[index] = 0;
        color_r = bgRed;
        color_g = bgGreen;
        color_b = bgBlue;
    } else {
        randomSparkRangeIndexs[index] = random(end - start) + start;
        randomSparkRangeFlags[index] = 1;
    }
    setRange(bgRed, bgGreen, bgBlue, start, end);
    if (random(255) < sparking && (randomSparkRangeIndexs[index] >= start && randomSparkRangeIndexs[index] <= end)) {
        pixels.setPixelColor(randomSparkRangeIndexs[index], color_r, color_g, color_b);
    }
    pixels.show();

}

/*
    neopixelSet1.fadeInOut(r, g, b, 3);
    neopixelSet1.fadeInOutRange(r, g, b, 1, 100, start, end);
    neopixelSet1.fadeInOutRange(r, g, b, 1, 100, start, end, index);

*/
void NeoPixelAnimations::fadeInOut(int red, int green, int blue, int speed) {
    int index = 0;
    int minBrightness = 0;
    fadeInOutRange(red, green, blue, speed, minBrightness, 0, numPixels, index);
}
void NeoPixelAnimations::fadeInOutRange(int red, int green, int blue, int speed, int minBrightness, int start, int end) {
    int index = 0;
    fadeInOutRange(red, green, blue, speed, minBrightness, start, end, index);
}
void NeoPixelAnimations::fadeInOutRange(int red, int green, int blue, int speed, int minBrightness, int start, int end, int index) {
    float r, g, b;
    r = (fadeInOutRangeCounters[index] / 255) * red;
    g = (fadeInOutRangeCounters[index] / 255) * green;
    b = (fadeInOutRangeCounters[index] / 255) * blue;
    setRange(round(r), round(g), round(b), start, end);
    if (fadeInOutRangeDirections[index] < 1) {
        fadeInOutRangeCounters[index] += speed;
    } else {
        fadeInOutRangeCounters[index] -= speed * 2;
    }
    if (fadeInOutRangeCounters[index] >= 255) {
        fadeInOutRangeDirections[index] = 1;
    } else if (fadeInOutRangeCounters[index] <= minBrightness) {
        fadeInOutRangeCounters[index] = minBrightness;
        fadeInOutRangeDirections[index] = 0;
    }
}

/*
    neopixelSet3.strobe(r, g, b, 5, 5);
    neopixelSet1.strobeRange(r, g, b, 5, 5, start, end);
    neopixelSet1.strobeRange(r, g, b, 5, 5, start, end, index);
*/
void NeoPixelAnimations::strobe(int red, int green, int blue, int totalStrobes, int delayPeriods) {
    int index = 0;
    strobeRange(red, green, blue, totalStrobes, delayPeriods, 0, numPixels, index);
}
void NeoPixelAnimations::strobeRange(int red, int green, int blue, int totalStrobes, int delayPeriods, int start, int end) {
    int index = 0;
    strobeRange(red, green, blue, totalStrobes, delayPeriods, start, end, index);
}
void NeoPixelAnimations::strobeRange(int red, int green, int blue, int totalStrobes, int delayPeriods, int start, int end, int index) {
    int r = red,
        g = green,
        b = blue;
    if (strobeOnRangeFlags[index] == 0) {
        strobeOnRangeFlags[index] = 1;
        r = 0;
        g = 0;
        b = 0;
    } else {
        strobeOnRangeFlags[index] = 0;
    }
    setRange(r, g, b, start, end);
    // pixels.show();
    strobeRangeCounters[index]++;
    if (strobeRangeCounters[index] > (totalStrobes * 2)) {
        strobeOnRangeFlags[index] = 0;
        if (strobeRangeCounters[index] >= (totalStrobes * delayPeriods)) {
            strobeOnRangeFlags[index] = 1;
            strobeRangeCounters[index] = 0;
        }
    }
}

/*
    neopixelSet1.cylonBounce(r, g, b, 4);
*/
void NeoPixelAnimations::cylonBounce(int red, int green, int blue, int eyeSize) {
    int index = 0;
    cylonBounceRange(red, green, blue, eyeSize, 0, numPixels, index);
}
void NeoPixelAnimations::cylonBounceRange(int red, int green, int blue, int eyeSize, int start, int end) {
    int index = 0;
    cylonBounceRange(red, green, blue, eyeSize, start, end, index);
}
void NeoPixelAnimations::cylonBounceRange(int red, int green, int blue, int eyeSize, int start, int end, int index) {
    setRange(0, 0, 0, start, end);
    for (int a = 2; a > 0; a--) {
        pixels.setPixelColor(cylonBounceOffset[index] - a, red / (a * 5), green / (a * 5), blue / (a * 5));
    }
    for (int j = 0; j < eyeSize; j++) {
        pixels.setPixelColor(cylonBounceOffset[index] + j, red, green, blue);
    }
    for (int b = 1; b <= 2; b++) {
        pixels.setPixelColor(cylonBounceOffset[index] - 1 + eyeSize + b, red / (b * 5), green / (b * 5), blue / (b * 5));
    }
    pixels.show();
    if (cylonBounceDirection[index] == 1) {
        cylonBounceOffset[index]--;
    } else {
        cylonBounceOffset[index]++;
    }
    if (cylonBounceOffset[index] > end - eyeSize - 2) {
        cylonBounceDirection[index] = 1;
    } else if (cylonBounceOffset[index] < start + 1) {
        cylonBounceDirection[index] = start;
    }
}

/*
    neopixelSet1.comet(r, g, b, 2, 8);
*/
void NeoPixelAnimations::comet(int red, int green, int blue, int cometSize, int cometTailSize) {
    int index = 0;
    cometRange(red, green, blue, cometSize, cometTailSize, 0, numPixels, index);
}
void NeoPixelAnimations::cometRange(int red, int green, int blue, int cometSize, int cometTailSize, int start, int end) {
    int index = 0;
    cometRange(red, green, blue, cometSize, cometTailSize, start, end, index);
}
void NeoPixelAnimations::cometRange(int red, int green, int blue, int cometSize, int cometTailSize, int start, int end, int index) {
    setRange(0, 0, 0, start, end);
    for (int a = 0; a < cometSize; a++) {
        pixels.setPixelColor(cometOffset[index] + a, red, green, blue);
    }
    for (int j = 0; j < cometTailSize; j++) {
        int bright = random(2, 100);
        if (cometDirection[index] == 1) {
            pixels.setPixelColor(cometOffset[index] + cometSize + j, red / bright, green / bright, blue / bright);
        } else {
            pixels.setPixelColor(cometOffset[index] - j, red / bright, green / bright, blue / bright);
        }
    }
    pixels.show();
    if (cometDirection[index] == 1) {
        cometOffset[index]--;
    } else {
        cometOffset[index]++;
    }
    if (cometOffset[index] > end - cometSize - 2) {
        cometDirection[index] = 1;
    } else if (cometOffset[index] < start + 1) {
        cometDirection[index] = 0;
    }
}

/*
    neopixelSet1.theaterChase(r, g, b, 1);
*/
void NeoPixelAnimations::theaterChase(int red, int green, int blue, int direction) {
    int index = 0;
    theaterChaseRange(red, green, blue, direction, 0, numPixels, index);
}
void NeoPixelAnimations::theaterChaseRange(int red, int green, int blue, int direction, int start, int end) {
    int index = 0;
    theaterChaseRange(red, green, blue, direction, start, end, index);
}
void NeoPixelAnimations::theaterChaseRange(int red, int green, int blue, int direction, int start, int end, int index) {
    for(int i = start; i <= end; i++) {
        pixels.setPixelColor(i,
            ((sin(i + theaterChaseRangeOffset[index]) * 127 + 128) / 255) * red,
            ((sin(i + theaterChaseRangeOffset[index]) * 127 + 128) / 255) * green,
            ((sin(i + theaterChaseRangeOffset[index]) * 127 + 128) / 255) * blue
        );
    }
    pixels.show();
    if (direction == 1) {
        theaterChaseRangeOffset[index]++;
        if (theaterChaseRangeOffset[index] > (end - start)) {
            theaterChaseRangeOffset[index] = 0;
        }
    } else {
        theaterChaseRangeOffset[index]--;
        if (theaterChaseRangeOffset[index] < 0) {
            theaterChaseRangeOffset[index] = (end - start);
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
    neopixelSet1.theaterChaseRainbow(1, 3, 10);
*/
void NeoPixelAnimations::theaterChaseRainbow(int direction, int speed, int brightness) {
    int index = 0;
    theaterChaseRainbowRange(direction, speed, brightness, 0, numPixels, index);
}
void NeoPixelAnimations::theaterChaseRainbowRange(int direction, int speed, int brightness, int start, int end) {
    int index = 0;
    theaterChaseRainbowRange(direction, speed, brightness, start, end, index);
}
void NeoPixelAnimations::theaterChaseRainbowRange(int direction, int speed, int brightness, int start, int end, int index) {
    byte *c;
    for(int i = start; i < end; i++) {
        c = wheel(((i * 256 / (end - start)) + theaterChaseRainbowWheelPos[index]) & 255);
        pixels.setPixelColor(i,
            ((sin(i + theaterChaseRainbowOffset[index]) * 127 + 128) / 255) * c[0] / brightness,
            ((sin(i + theaterChaseRainbowOffset[index]) * 127 + 128) / 255) * c[1] / brightness,
            ((sin(i + theaterChaseRainbowOffset[index]) * 127 + 128) / 255) * c[2] / brightness
        );
    }
    pixels.show();
    if (direction == 1) {
        theaterChaseRainbowOffset[index]++;
        if (theaterChaseRainbowOffset[index] > (end - start)) {
            theaterChaseRainbowOffset[index] = 0;
        }
        theaterChaseRainbowWheelPos[index] += speed;
        if (theaterChaseRainbowWheelPos[index] > 256 * 5) {
            theaterChaseRainbowWheelPos[index] = 0;
        }
    } else {
        theaterChaseRainbowOffset[index]--;
        if (theaterChaseRainbowOffset[index] < 0) {
            theaterChaseRainbowOffset[index] = (end - start);
        }
        theaterChaseRainbowWheelPos[index] -= speed;
        if (theaterChaseRainbowWheelPos[index] < 0) {
            theaterChaseRainbowWheelPos[index] = 256 * 5;
        }
    }
}

/*
  Cycle clockwise:
      neopixelSet1.rainbowCycle(1, 3, 10);
  Cycle counter clockwise:
      neopixelSet1.rainbowCycle(0, 3, 10);
*/
void NeoPixelAnimations::rainbowCycle(int direction, int speed, int brightness) {
    int index = 0;
    rainbowCycleRange(direction, speed, brightness, 0, numPixels, index);
}
void NeoPixelAnimations::rainbowCycleRange(int direction, int speed, int brightness, int start, int end) {
    int index = 0;
    rainbowCycleRange(direction, speed, brightness, start, end, index);
}
void NeoPixelAnimations::rainbowCycleRange(int direction, int speed, int brightness, int start, int end, int index) {
    byte *c;
    for(int i = start; i < end; i++) {
        // c = wheel(rainbowCycleWheelPos + i);
        c = wheel(((i * 256 / (end - start)) + rainbowCycleWheelPos[index]) & 255);
        pixels.setPixelColor(i, c[0]/brightness, c[1]/brightness, c[2]/brightness);
    }
    pixels.show();
    if (direction == 1) {
        rainbowCycleWheelPos[index] += speed;
        if (rainbowCycleWheelPos[index] > 256 * 5) {
            rainbowCycleWheelPos[index] = 0;
        }
    } else {
        rainbowCycleWheelPos[index] -= speed;
        if (rainbowCycleWheelPos[index] < 0) {
            rainbowCycleWheelPos[index] = 256 * 5;
        }
    }

}

/*
    neopixelSet1.setPixelHeatColor(j, heat[j]);
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
  Random fire for a NeoPixel circle with 24 pixels
    neopixelSet1.fire(80, 80);
*/
void NeoPixelAnimations::fire(int cooling, int sparking) {
    int index = 0;
    fireRange(cooling, sparking, 0, numPixels, index);
}
void NeoPixelAnimations::fireRange(int cooling, int sparking, int start, int end) {
    int index = 0;
    fireRange(cooling, sparking, start, end, index);
}
void NeoPixelAnimations::fireRange(int cooling, int sparking, int start, int end, int index) {
    int cooldown;
    // heat[pixelCount];
    if (fireInit[index] != 1) {
        heat = new byte[end-start];
        fireInit[index] = 1;
    }
    // Step 1.  Cool down every cell a little
    for (int i = start; i < end; i++) {
        cooldown = random(start, ((cooling * 10) / end) + 2);
        if(cooldown > heat[i]) {
            heat[i] = 0;
        } else {
            heat[i] = heat[i] - cooldown;
        }
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = end - 1; k >= start; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < sparking) {
        int y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
    }
    // Step 4.  Convert heat to LED colors
    for (int j = start; j < end; j++) {
        setPixelHeatColor(j, heat[j]);
    }
    pixels.show();
}

/*
    neopixelSet1.bouncing(255, 20, 147, 4);
*/
int NeoPixelAnimations::bouncing(int red, int green, int blue, int objectCount, int randomColor) {
    if (bouncingInit != 1) {
        bouncingPosition = new int[objectCount];
        bouncingZPosition = new float[objectCount];
        bouncingHeight = new float[objectCount];
        bouncingImpactVelocity = new float[objectCount];
        bouncingTimeSinceLastBounce = new float[objectCount];
        bouncingDampening = new float[objectCount];
        bouncingClockTimeSinceLastBounce = new long[objectCount];
        bouncingColor = new int*[objectCount];

        for (int i = 0; i < objectCount; i++) {
            bouncingClockTimeSinceLastBounce[i] = millis();
            bouncingHeight[i] = bouncingStartHeight;
            bouncingPosition[i] = 0;
            bouncingZPosition[i] = 1;
            bouncingImpactVelocity[i] = bouncingimpactVelocityStart;
            bouncingTimeSinceLastBounce[i] = 0;
            bouncingDampening[i] = 0.90 - float(i) / pow(objectCount, 2);
            bouncingColor[i] = new int[3];
            if (randomColor == 1) {
                bouncingColor[i][0] = random(50);
                bouncingColor[i][1] = random(50);
                bouncingColor[i][2] = random(50);
            } else {
                bouncingColor[i][0] = red;
                bouncingColor[i][1] = green;
                bouncingColor[i][2] = blue;
            }
        }
        bouncingInit = 1;
    }
    for (int i = 0; i < objectCount; i++) {
        bouncingTimeSinceLastBounce[i] =  millis() - bouncingClockTimeSinceLastBounce[i];
        bouncingHeight[i] = 0.5 *
            bouncingGravity * pow(bouncingTimeSinceLastBounce[i] / 1000, 2.0) +
            bouncingImpactVelocity[i] * bouncingTimeSinceLastBounce[i] / 1000;
            bouncingZPosition[i] += 0.01;
        if (bouncingHeight[i] < 0) {
            bouncingZPosition[i] = 1;
            bouncingHeight[i] = 0;
            bouncingImpactVelocity[i] = bouncingDampening[i] * bouncingImpactVelocity[i];
            bouncingClockTimeSinceLastBounce[i] = millis();

            if (bouncingImpactVelocity[i] < 0.1) {
                bouncingImpactVelocity[i] = bouncingimpactVelocityStart;
            }
        }
        bouncingPosition[i] = round(bouncingHeight[i] * (numPixels - 1) / bouncingStartHeight);
    }

    setAll(0, 0, 0);
    for (int i = 0; i < objectCount; i++) {
        // pixels.setPixelColor(bouncingPosition[i], red, green, blue);
        pixels.setPixelColor(
            bouncingPosition[i],
            round(bouncingColor[i][0] / bouncingZPosition[i]),
            round(bouncingColor[i][1] / bouncingZPosition[i]),
            round(bouncingColor[i][2] / bouncingZPosition[i])
        );
    }
    pixels.show();

    return bouncingPosition[0];
}

// TODO:
//   fillFromOneEnd(r, g, b, speed);
//   fillMixedColors() // Red, green, white // https://www.youtube.com/watch?v=VkFwQ9gey74
//   fillDotsUntilFull
//   fillWith2Colors // Red and white like this video: https://www.youtube.com/watch?v=76HUAiYQ6DM
//   randomSpark with different background colors and with the blink i the same color as the background.
//    // https://www.youtube.com/watch?v=-EkgT_sNYDY
// https://www.youtube.com/watch?v=2W7L41D_2ns
// https://www.youtube.com/watch?v=bb-4UADvVHw
// http://dougalcampbell.github.io/LEDStrip/
// http://rurandom.org/justintime/index.php?title=Driving_the_WS2811_at_800_kHz_with_an_8_MHz_AVR

// fire
// int r = 255;
// int g = r-40;
// int b = 40;
//
// for(int x = 8; x <99; x++)
// {
// int flicker = random(0,150);
// int r1 = r-flicker;
// int g1 = g-flicker;
// int b1 = b-flicker;
// if(g1<0) g1=0;
// if(r1<0) r1=0;
// if(b1<0) b1=0;
// strip.setPixelColor(x,r1,g1, b1);
// }
// strip.show();
// delay(random(50,150));

