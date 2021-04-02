#ifndef PICOLEDEFFECT_COMET_H
#define PICOLEDEFFECT_COMET_H

#include <vector>
#include "Effects/Fade.hpp"

using std::vector;

namespace PicoLed {

class Comet: public Fade {

    public:
        Comet(PicoLedController &controller, Color color, double speed, double length);
        Comet(PicoLedController &controller, Color color, double speed, double length, double fadeRate);

    protected:
        Color color;
        double offset;
        double speed;
        double length;

        bool fadePixels(uint32_t timeNow);
        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif