#ifndef PICOLEDEFFECT_MARQUEE_H
#define PICOLEDEFFECT_MARQUEE_H

#include "PicoLedEffect.hpp"
#include <vector>

using std::vector;

namespace PicoLed {

class Marquee: public PicoLedEffect {

    public:
        Marquee(PicoLedController &controller, vector<Color> palette, double length, double speed);
        Marquee(PicoLedController &controller, vector<Color> palette, double length, double speed, double spacing);

    protected:
        vector<Color> palette;
        double length;
        double spacing;
        double offset;
        double speed;

        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif