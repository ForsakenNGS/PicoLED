#ifndef PICOLED_H
#define PICOLED_H

#include <stdexcept>
#include <cmath>
#include <map>
#include "PicoLedController.hpp"
#include "WS2812B.hpp"

using std::map;
using std::shared_ptr;

namespace PicoLed {

    class InvalidPioBlock: public std::exception {
    };

    class StateMachineInUse: public std::exception {
    };

    static map<uint, map<uint, shared_ptr<PicoLedTarget>>> targets;

    template<typename T>
    static PicoLedController addLeds(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4) {
        uint pioIndex;
        if (pioBlock == pio0) {
            pioIndex = 0;
        } else if (pioBlock == pio1) {
            pioIndex = 1;
        } else {
            throw new InvalidPioBlock();
        }
        if (targets[pioIndex].find(stateMachine) != targets[pioIndex].end()) {
            throw new StateMachineInUse();
        }
        shared_ptr<T> target(new T(pioBlock, stateMachine, dataPin, numLeds, b1, b2, b3, b4));
        targets[pioIndex][stateMachine] = target;
        return PicoLedController(target);
    };

    template<typename T>
    static PicoLedController addLeds(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataFormat format) {
        switch (format) {
            case FORMAT_RGB:
                return addLeds<T>(pioBlock, stateMachine, dataPin, numLeds, RED, GREEN, BLUE, NONE);
            default:
            case FORMAT_GRB:
                return addLeds<T>(pioBlock, stateMachine, dataPin, numLeds, GREEN, RED, BLUE, NONE);
            case FORMAT_WRGB:
                return addLeds<T>(pioBlock, stateMachine, dataPin, numLeds, RED, GREEN, BLUE, WHITE);
        }
    }

    static Color RGB(uint8_t red, uint8_t green, uint8_t blue) {
        return (struct Color){ .red = red, .green = green, .blue = blue, .white = std::min(std::min(red, green), blue) };
    };

    static Color RGBW(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
        return (struct Color){ .red = red, .green = green, .blue = blue, .white = white };
    }

    static Color HSV(uint8_t hue, uint8_t saturation, uint8_t value) {
        if (saturation == 0) {
            return (struct Color){ .red = value, .green = value, .blue = value, .white = 0 };
        }
        uint8_t quadrant = hue / 43;
        uint8_t remainder = hue - quadrant * 43 * 6;
        uint8_t p = (value * (255 - saturation)) >> 8;
        uint8_t q = (value * (255 - ((saturation * remainder) >> 8))) >> 8;
        uint8_t t = (value * (255 - ((saturation * (255 - remainder)) >> 8))) >> 8;
        switch (quadrant) {
            case 0:
                return (struct Color){ .red = value, .green = t, .blue = p, .white = std::min(t, p) };
            case 1:
                return (struct Color){ .red = q, .green = value, .blue = p, .white = std::min(q, p) };
            case 2:
                return (struct Color){ .red = p, .green = value, .blue = t, .white = std::min(t, p) };
            case 3:
                return (struct Color){ .red = p, .green = q, .blue = value, .white = std::min(q, p) };
            case 4:
                return (struct Color){ .red = t, .green = p, .blue = value, .white = std::min(t, p) };
            default:
            case 5:
                return (struct Color){ .red = value, .green = p, .blue = q, .white = std::min(q, p) };
        }
    }

    static Color MixColors(Color colorA, Color colorB, double ratio) {
        return (struct Color){ 
            .red = (uint8_t)((double)colorA.red * ratio + (double)colorB.red * (1.0 - ratio)),
            .green = (uint8_t)((double)colorA.green * ratio + (double)colorB.green * (1.0 - ratio)),
            .blue = (uint8_t)((double)colorA.blue * ratio + (double)colorB.blue * (1.0 - ratio)),
            .white = (uint8_t)((double)colorA.white * ratio + (double)colorB.white * (1.0 - ratio)),
        };
    }
}

#endif