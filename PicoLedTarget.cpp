#include "PicoLed.hpp"
#include "PicoLedTarget.hpp"
#include <stdio.h>

using std::min;

namespace PicoLed {

PicoLedTarget::PicoLedTarget(uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4): numLeds(numLeds), brightness(255)
{
    bytes[0] = b1;
    bytes[1] = b2;
    bytes[2] = b3;
    bytes[3] = b4;
}

PicoLedTarget::~PicoLedTarget() {
}

uint PicoLedTarget::getNumLeds() {
    return numLeds;
}

uint8_t PicoLedTarget::getBrightness() {
    return brightness;
}

void PicoLedTarget::setBrightness(uint8_t brightness) {
    printf("Brightness: %u at %X\n", brightness, this);
    this->brightness = brightness;
}

Color PicoLedTarget::getPixelColor(uint index) {
    Color pixelColor = RGBW(0, 0, 0, 0);
    uint32_t pixelData = getData(index);
    for (uint b = 4; b-- > 0; ) {
        switch (bytes[b]) {
            case RED:
                pixelColor.red = pixelData & 0xFF;
                break;
            case GREEN:
                pixelColor.green = pixelData & 0xFF;
                break;
            case BLUE:
                pixelColor.blue = pixelData & 0xFF;
                break;
            case WHITE:
                pixelColor.white = pixelData & 0xFF;
                break;
        }
        pixelData >>= 8;
    }
    return pixelColor;
}

void PicoLedTarget::setPixelColor(uint index, Color color) {
    setData(index, getPixelData(color));
}

void PicoLedTarget::fill(Color color, uint first, uint count) {
    uint32_t colorData = getPixelData(color);
    uint last = (first + count);
    if (last > numLeds) {
        last = numLeds;
    }
    for (uint i = first; i < last; i++) {
        setData(i, colorData);
    }
}

uint32_t PicoLedTarget::getPixelData(Color color) {
    uint32_t result = 0;
    for (uint b = 0; b < 4; b++) {
        result <<= 8;
        switch (bytes[b]) {
            case RED:
                result |= color.red;
                break;
            case GREEN:
                result |= color.green;
                break;
            case BLUE:
                result |= color.blue;
                break;
            case WHITE:
                result |= color.white;
                break;
        }
    }
    return result;
}

uint32_t PicoLedTarget::scalePixelData(uint32_t data, uint16_t scale) {
    uint32_t dataA = (data & 0x00FF00FF) * scale;
    uint32_t dataB = ((data >> 8) & 0x00FF00FF) * scale;
    return ((dataA >> 8) & 0x00FF00FF | (dataB & 0xFF00FF00));
}

uint32_t PicoLedTarget::getData(uint index) {
    return 0;
}

void PicoLedTarget::setData(uint index, uint32_t value) {
}

void PicoLedTarget::show() {
}

}