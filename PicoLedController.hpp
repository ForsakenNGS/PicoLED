#ifndef PICOLEDCONTROLLER_H
#define PICOLEDCONTROLLER_H

#include <cmath>
#include <algorithm>
#include <memory>
#include "pico/types.h"
#include "hardware/pio.h"
#include "PicoLedTarget.hpp"

using std::shared_ptr;

namespace PicoLed {

enum DrawMode {
    MODE_SET=0,
    MODE_ADD=1,
    MODE_SUB=2
};

class PicoLedController {
    public:
        PicoLedController(shared_ptr<PicoLedTarget> target);
        PicoLedController(PicoLedController& target, uint start, uint end);
        ~PicoLedController();

        uint getNumLeds();
        uint8_t getBrightness();
        void setBrightness(uint8_t brightness);
        DrawMode getDrawMode();
        void setDrawMode(DrawMode mode);
        Color getPixelColor(uint index);
        void setPixelColor(uint index, Color color);
        void setPixelColor(uint index, Color color, DrawMode mode);
        void show();
        void clear();
        void clear(Color color);
        void fill(Color color);
        void fill(Color color, uint first);
        void fill(Color color, uint first, uint count);
        void fillGradient(Color colorStart, Color colorEnd);
        void fillGradient(Color colorStart, Color colorEnd, uint first);
        void fillGradient(Color colorStart, Color colorEnd, uint first, uint count);
        void fillRainbow(uint8_t initialHue, uint8_t deltaHue);
        void fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first);
        void fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first, uint count);
        void fade(Color color, double factor);
        void fade(Color color, uint first, double factor);
        void fade(Color color, uint first, uint count, double factor);
        void fadeLine(Color color, double first, double factor);
        void fadeLine(Color color, double first, double count, double factor);
        void fadePixel(uint index, Color color, double factor);
        void fadeValue(Color color, uint8_t value);
        void fadeValue(Color color, uint first, uint8_t value);
        void fadeValue(Color color, uint first, uint count, uint8_t value);
        void fadePixelValue(uint index, Color color, uint8_t value);
        PicoLedController slice(uint start, uint end);
    protected:
        shared_ptr<PicoLedTarget> target;
        DrawMode mode;
};

};

#endif