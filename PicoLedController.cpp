#include "PicoLed.hpp"
#include "PicoLedController.hpp"
#include "VirtualStrip.hpp"

using std::min;
using std::shared_ptr;

namespace PicoLed {

PicoLedController::PicoLedController(shared_ptr<PicoLedTarget> target): target(target), mode(MODE_SET)
{
}

PicoLedController::PicoLedController(PicoLedController& controller, uint start, uint end):
target(new VirtualStrip(controller.target, start, end)), mode(MODE_SET)
{
}

PicoLedController::~PicoLedController() {
}

uint PicoLedController::getNumLeds() {
    return target->getNumLeds();
}

uint8_t PicoLedController::getBrightness() {
    return target->getBrightness();
}

void PicoLedController::setBrightness(uint8_t brightness) {
    target->setBrightness(brightness);
}

DrawMode PicoLedController::getDrawMode() {
    return mode;
}

void PicoLedController::setDrawMode(DrawMode mode) {
    this->mode = mode;
}

void PicoLedController::setPixelColor(uint index, Color color) {
    setPixelColor(index, color, mode);
}

void PicoLedController::setPixelColor(uint index, Color color, DrawMode mode) {
    switch (mode) {
        default:
        case MODE_SET:
        {
            target->setPixelColor(index, color);
            break;
        }
        case MODE_ADD:
        {
            Color mixed = target->getPixelColor(index);
            mixed.red += min<uint8_t>(color.red, 255 - mixed.red);
            mixed.green += min<uint8_t>(color.green, 255 - mixed.green);
            mixed.blue += min<uint8_t>(color.blue, 255 - mixed.blue);
            mixed.white += min<uint8_t>(color.white, 255 - mixed.white);
            target->setPixelColor(index, mixed);
            break;
        }
        case MODE_SUB:
        {
            Color mixed = target->getPixelColor(index);
            mixed.red -= min<uint8_t>(color.red, mixed.red);
            mixed.green -= min<uint8_t>(color.green, mixed.green);
            mixed.blue -= min<uint8_t>(color.blue, mixed.blue);
            mixed.white -= min<uint8_t>(color.white, mixed.white);
            target->setPixelColor(index, mixed);
            break;
        }
    }
}

void PicoLedController::clear() {
    clear(RGBW(0, 0, 0, 0));
}

void PicoLedController::clear(Color color) {
    target->fill(color, 0, target->getNumLeds());
}

void PicoLedController::fill(Color color) {
    fill(color, 0, target->getNumLeds());
}

void PicoLedController::fill(Color color, uint first) {
    fill(color, first, target->getNumLeds() - first);
}

void PicoLedController::fill(Color color, uint first, uint count) {
    target->fill(color, first, count);
}

void PicoLedController::fillGradient(Color colorStart, Color colorEnd) {
    fillGradient(colorStart, colorEnd, 0, target->getNumLeds());
}

void PicoLedController::fillGradient(Color colorStart, Color colorEnd, uint first) {
    fillGradient(colorStart, colorEnd, first, target->getNumLeds() - first);
}

void PicoLedController::fillGradient(Color colorStart, Color colorEnd, uint first, uint count) {
    uint last = (first + count);
    if (last > target->getNumLeds()) {
        last = target->getNumLeds();
    }
    for (uint i = first; i < last; i++) {
        double ratio = (double)(i - first) / (double)(last - first);
        setPixelColor(i, MixColors(colorStart, colorEnd, ratio));
    }
}

void PicoLedController::fillRainbow(uint8_t initialHue, uint8_t deltaHue) {
    fillRainbow(initialHue, deltaHue, 0, target->getNumLeds());
}

void PicoLedController::fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first) {
    fillRainbow(initialHue, deltaHue, first, target->getNumLeds() - first);
}

void PicoLedController::fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first, uint count) {
    uint last = (first + count);
    if (last > target->getNumLeds()) {
        last = target->getNumLeds();
    }
    for (uint i = first; i < last; i++) {
        setPixelColor(i, HSV(initialHue, 255, 255));
        initialHue += deltaHue;
    }
}

void PicoLedController::fade(Color color, double factor) {
    fade(color, 0, target->getNumLeds(), factor);
}

void PicoLedController::fade(Color color, uint first, double factor) {
    fade(color, first, target->getNumLeds() - first, factor);
}

void PicoLedController::fade(Color color, uint first, uint count, double factor) {
    uint last = first + count;
    if (last > target->getNumLeds()) {
        last = target->getNumLeds();
    }
    for (uint i = first; i < last; i++) {
        fadePixel(i, color, factor);
    }
}

void PicoLedController::fadeLine(Color color, double first, double factor) {
    fadeLine(color, first, (double)target->getNumLeds() - first, factor);
}

void PicoLedController::fadeLine(Color color, double first, double count, double factor) {
    if (first < 0.0) {
        // Clamp to start
        count += first;
        first = 0.0;
    }
    if ((first + count) > target->getNumLeds()) {
        // Clamp to end
        count = (double)target->getNumLeds() - first;
    }
    if (count <= 0) {
        // Out of bounds / zero length
        return;
    }
    double last = first + count;
    if (last <= ceil(first)) {
        // Single pixel
        fadePixel(floor(first), color, (last - first) * factor);
    } else {
        double middleStart = ceil(first);
        double middleEnd = floor(last);
        // First pixel
        fadePixel(floor(first), color, (middleStart - first) * factor);
        // Middle section
        if (middleEnd - middleStart >= 1.0) {
            fade(color, middleStart, middleEnd - middleStart, factor);
        }
        // Last pixel
        if (last > middleEnd) {
            fadePixel(middleEnd, color, (last - middleEnd) * factor);
        }
    }
}

void PicoLedController::fadePixel(uint index, Color color, double factor) {
    switch (mode) {
        default:
        case MODE_SET:
        {
            Color pixelColor = target->getPixelColor(index);
            setPixelColor(index, MixColors(color, pixelColor, factor));
            break;
        }
        case MODE_ADD:
        case MODE_SUB:
        {
            setPixelColor(index, MixColors(color, RGB(0, 0, 0), factor));
            break;
        }
    }
}

void PicoLedController::fadeValue(Color color, uint8_t value) {
    fadeValue(color, 0, target->getNumLeds(), value);
}

void PicoLedController::fadeValue(Color color, uint first, uint8_t value) {
    fadeValue(color, first, target->getNumLeds() - first, value);
}

void PicoLedController::fadeValue(Color color, uint first, uint count, uint8_t value) {
    uint last = first + count;
    if (last > target->getNumLeds()) {
        last = target->getNumLeds();
    }
    for (uint i = first; i < last; i++) {
        fadePixelValue(i, color, value);
    }
}

void PicoLedController::fadePixelValue(uint index, Color color, uint8_t value) {
    Color pixelColor = target->getPixelColor(index);
    int diffRed = (int)color.red - (int)pixelColor.red;
    int diffGreen = (int)color.green - (int)pixelColor.green;
    int diffBlue = (int)color.blue - (int)pixelColor.blue;
    int diffWhite = (int)color.white - (int)pixelColor.white;
    if (abs(diffRed) <= value) {
        pixelColor.red = color.red;
    } else if (diffRed < 0) {
        pixelColor.red -= value;
    } else {
        pixelColor.red += value;
    }
    if (abs(diffGreen) <= value) {
        pixelColor.green = color.green;
    } else if (diffGreen < 0) {
        pixelColor.green -= value;
    } else {
        pixelColor.green += value;
    }
    if (abs(diffBlue) <= value) {
        pixelColor.blue = color.blue;
    } else if (diffBlue < 0) {
        pixelColor.blue -= value;
    } else {
        pixelColor.blue += value;
    }
    if (abs(diffWhite) <= value) {
        pixelColor.white = color.white;
    } else if (diffWhite < 0) {
        pixelColor.white -= value;
    } else {
        pixelColor.white += value;
    }
    setPixelColor(index, pixelColor, MODE_SET);
}

void PicoLedController::show() {
    target->show();
}

PicoLedController PicoLedController::slice(uint start, uint end) {
    return PicoLedController(*this, start, end);
}

}