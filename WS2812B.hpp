#ifndef WS2812B_H
#define WS2812B_H

#include "PioStrip.hpp"

namespace PicoLed {

class WS2812B: public PioStrip {

    public:
        WS2812B(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4);

};

}

#endif