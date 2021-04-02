#ifndef PICOLEDEFFECT_H
#define PICOLEDEFFECT_H

#include <pico/types.h>
#include <pico/time.h>
#include "PicoLedController.hpp"

namespace PicoLed {

class PicoLedEffect {

    public:
        PicoLedEffect(PicoLedController &controller);
        virtual ~PicoLedEffect();

        bool animate();
        virtual void reset();

    protected:
        PicoLedController controller;
        uint32_t lastUpdate;

        virtual bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif