#ifndef NEOPIXEL_ANIMATIONS_H
#define NEOPIXEL_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class NeoPixelAnimations {
    Adafruit_NeoPixel pixels;
    int
        numPixels,
        // runningLight
        *runningLightRangeOffsets,
        // randomSparkRange
        *randomSparkRangeFlags,
        *randomSparkRangeIndexs,
        // fadeInOutRange
        *fadeInOutRangeDirections,
        // strobeRange
        *strobeRangeCounters,
        *strobeOnRangeFlags,
        // cylonBounce
        *cylonBounceOffset,
        *cylonBounceDirection,
        // comet
        *cometOffset,
        *cometDirection,
        // theaterChase
        *theaterChaseRangeOffset,
        // theaterChaseRainbow
        *theaterChaseRainbowOffset,
        *theaterChaseRainbowWheelPos,
        // rainbowCycle
        *rainbowCycleWheelPos,
        // fire
        *fireInit,
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
        void cylonBounceRange(int red, int green, int blue, int eyeSize, int start, int end);
        void cylonBounceRange(int red, int green, int blue, int eyeSize, int start, int end, int index);
        void comet(int red, int green, int blue, int cometSize, int cometTailSize);
        void cometRange(int red, int green, int blue, int cometSize, int cometTailSize, int start, int end);
        void cometRange(int red, int green, int blue, int cometSize, int cometTailSize, int start, int end, int index);
        void theaterChase(int red, int green, int blue, int direction);
        void theaterChaseRange(int red, int green, int blue, int direction, int start, int end);
        void theaterChaseRange(int red, int green, int blue, int direction, int start, int end, int index);
        void theaterChaseRainbow(int direction, int speed, int brightness);
        void theaterChaseRainbowRange(int direction, int speed, int brightness, int start, int end);
        void theaterChaseRainbowRange(int direction, int speed, int brightness, int start, int end, int index);
        byte* wheel(int wheelPos);
        void rainbowCycle(int direction, int speed, int brightness);
        void rainbowCycleRange(int direction, int speed, int brightness, int start, int end);
        void rainbowCycleRange(int direction, int speed, int brightness, int start, int end, int index);
        void fire(int cooling, int sparking);
        void fireRange(int cooling, int sparking, int start, int end);
        void fireRange(int cooling, int sparking, int start, int end, int index);
        void setPixelHeatColor(int Pixel, byte temperature);
        int bouncing(int red, int green, int blue, int objectCount, int randomColor);
};

#endif // NEOPIXEL_ANIMATIONS_H
