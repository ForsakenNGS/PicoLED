#ifndef PICOLEDTARGET_H
#define PICOLEDTARGET_H

#include <cmath>
#include <algorithm>
#include "pico/types.h"

namespace PicoLed {

enum DataByte {
    NONE=0,
    RED=1,
    GREEN=2,
    BLUE=3,
    WHITE=4
};
enum DataFormat {
    FORMAT_RGB=0,
    FORMAT_GRB=1,
    FORMAT_WRGB=2
};
struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
};

class PicoLedTarget {
    public:
        PicoLedTarget(uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        ~PicoLedTarget();

        virtual uint getNumLeds();
        virtual uint8_t getBrightness();
        virtual void setBrightness(uint8_t brightness);
        virtual uint32_t getData(uint index);
        virtual void setData(uint index, uint32_t value);
        virtual void show();
        Color getPixelColor(uint index);
        void setPixelColor(uint index, Color color);
        void fill(Color color, uint first, uint count);
    protected:
        uint32_t getPixelData(Color color);
        uint32_t scalePixelData(uint32_t data, uint16_t scale);

        uint numLeds;
        DataByte bytes[4];
        uint8_t brightness;
};

};

#endif