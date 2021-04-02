#ifndef PICOLEDEFFECT_BOUNCE_H
#define PICOLEDEFFECT_BOUNCE_H

#include <vector>
#include "Effects/Fade.hpp"

using std::vector;

namespace PicoLed {

class Bounce: public Fade {

    public:
        Bounce(PicoLedController &controller, double fadeRate, double gravity);

        void addBall(Color color, double length);
        void reset();
    protected:
        struct BounceBall {
            Color color;
            double length;
            double offset;
            double speed;
        };
        vector<BounceBall> balls;
        double gravity;

        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif