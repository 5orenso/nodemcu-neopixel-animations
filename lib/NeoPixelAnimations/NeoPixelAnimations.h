#ifndef NEOPIXEL_ANIMATIONS_H
#define NEOPIXEL_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class NeoPixelAnimations {
    Adafruit_NeoPixel pixels;
    int
        numPixels,
        // runningLight
        runningLightOffset,
        *runningLightRangeOffsets,
        // randomSpark
        randomSparkFlag,
        randomSparkIndex,
        // randomSparkRange
        // randomSparkRangeFlag,
        // randomSparkRangeIndex,
        *randomSparkRangeFlags,
        *randomSparkRangeIndexs,
        // fadeInOut
        fadeInOutDirection,
        *fadeInOutRangeDirections,
        // strobe
        strobeCounter,
        strobeOnFlag,
        // strobeRange
        *strobeRangeCounters,
        *strobeOnRangeFlags,
        // cylonBounce
        cylonBounceOffset,
        cylonBounceDirection,
        // comet
        cometOffset,
        cometDirection,
        // theaterChase
        theaterChaseOffset,
        // theaterChaseRainbow
        theaterChaseRainbowOffset,
        theaterChaseRainbowWheelPos,
        // rainbowCycle
        rainbowCycleWheelPos,
        // fire
        fireInit,
        // bouncing
        bouncingInit,
        bouncingStartHeight,
        *bouncingPosition,
        **bouncingColor
        ;
    float
        // fadeInOut
        fadeInOutCounter,
        *fadeInOutRangeCounters,
        // bouncing
        bouncingGravity,
        bouncingimpactVelocityStart,
        *bouncingZPosition,
        *bouncingHeight,
        *bouncingImpactVelocity,
        *bouncingTimeSinceLastBounce,
        *bouncingDampening
        ;
    long
        // bouncing
        *bouncingClockTimeSinceLastBounce
        ;
    byte
        // fire
        *heat
        ;

    public:
        NeoPixelAnimations(Adafruit_NeoPixel &pixelsInput, int pixelCount);
        void runningLight(int red, int green, int blue, int sizeOfSnake);
        void runningLightRange(int red, int green, int blue, int start, int end, int sizeOfSnake);
        void runningLightRange(int red, int green, int blue, int start, int end, int sizeOfSnake, int index);
        void randomSpark(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking);
        void randomSparkRange(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking, int start, int end);
        void randomSparkRange(int red, int green, int blue, int bgRed, int bgGreen, int bgBlue, int sparking, int start, int end, int index);
        void setAll(int red, int green, int blue);
        void setRange(int red, int green, int blue, int start, int end);
        void fadeInOut(int red, int green, int blue, int speed);
        void fadeInOutRange(int red, int green, int blue, int speed, int minBrightness, int start, int end);
        void fadeInOutRange(int red, int green, int blue, int speed, int minBrightness, int start, int end, int index);
        void strobe(int red, int green, int blue, int totalStrobes, int delayPeriods);
        void strobeRange(int red, int green, int blue, int totalStrobes, int delayPeriods, int start, int end);
        void strobeRange(int red, int green, int blue, int totalStrobes, int delayPeriods, int start, int end, int index);
        void cylonBounce(int red, int green, int blue, int eyeSize);
        void comet(int red, int green, int blue, int cometSize, int cometTailSize);
        void theaterChase(int red, int green, int blue, int direction);
        void theaterChaseRainbow(int direction, int speed, int brightness);
        byte* wheel(int wheelPos);
        void rainbowCycle(int direction, int speed, int brightness);
        void fire(int cooling, int sparking);
        void setPixelHeatColor(int Pixel, byte temperature);
        int bouncing(int red, int green, int blue, int objectCount, int randomColor);
};

#endif // NEOPIXEL_ANIMATIONS_H
