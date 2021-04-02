#ifndef PICOLEDEFFECT_STARS_H
#define PICOLEDEFFECT_STARS_H

#include "Effects/Fade.hpp"
#include <vector>

using std::vector;

namespace PicoLed {

class Stars: public Fade {

    public:
        Stars(PicoLedController &controller, Color color, double spawnRate);
        Stars(PicoLedController &controller, Color color, double spawnRate, double fadeRate);
        Stars(PicoLedController &controller, vector<Color> palette, double spawnRate);
        Stars(PicoLedController &controller, vector<Color> palette, double spawnRate, double fadeRate);

    protected:
        vector<Color> palette;
        double spawnRate;
        uint32_t nextStar;

        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif