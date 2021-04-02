#ifndef PICOLEDEFFECT_FADE_H
#define PICOLEDEFFECT_FADE_H

#include "PicoLedEffect.hpp"

namespace PicoLed {

class Fade: public PicoLedEffect {

    public:
        Fade(PicoLedController &controller, Color color, double fadeRate);

    protected:
        Color fadeColor;
        double fadeRate;
        uint32_t fadeLastTick;

        bool fadePixels(uint32_t timeNow);
        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif