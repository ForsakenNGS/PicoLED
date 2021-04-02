#include "Comet.hpp"
#include "PicoLed.hpp"

using std::max;

namespace PicoLed {

    Comet::Comet(PicoLedController &controller, Color color, double speed, double length):
    Fade(controller, RGB(0, 0, 0), 0.5), color(color), offset(0.0), speed(speed), length(length)
    {
    }

    Comet::Comet(PicoLedController &controller, Color color, double speed, double length, double fadeRate):
    Fade(controller, RGB(0, 0, 0), fadeRate), color(color), offset(0.0), speed(speed), length(length)
    {
    }

    bool Comet::fadePixels(uint32_t timeNow) {
        uint32_t timeGone = timeNow - fadeLastTick;
        uint32_t fadeInterval = max<uint32_t>(1, 1000.0 / 255.0 / fadeRate);
        uint8_t fadeValue = timeGone / fadeInterval;
        if (fadeValue > 12) {
            uint8_t fadeValue = timeGone / fadeInterval;
            uint numLeds = controller.getNumLeds();
            for (uint i = 0; i < numLeds; i++) {
                if (random() % 2) {
                    controller.fadePixelValue(i, fadeColor, fadeValue);
                }
            }
            fadeLastTick = timeNow - (timeGone % fadeInterval);
            return true;
        } else {
            return false;
        }
    }

    bool Comet::update(uint32_t timeGone, uint32_t timeNow) {
        // Fade tail
        fadePixels(timeNow);
        // Draw body
        controller.fadeLine(color, offset, length, 1.0);
        // Calculate movement
        uint numLeds = controller.getNumLeds();
        offset += (double)timeGone * speed / 1000.0;
        if (offset > (double)numLeds - 1.0) {
            speed *= -1.0;
            offset = (double)numLeds - 1.0;
        } else if (offset + length < 1.0) {
            speed *= -1.0;
            offset = 1.0 - length;
        }
        return true;
    }

}