#include "WS2812B.hpp"
#include "WS2812B.pio.h"

namespace PicoLed {

WS2812B::WS2812B(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4): 
PioStrip(pioBlock, stateMachine, dataPin, numLeds, b1, b2, b3, b4)
{
    uint offset = pio_add_program(pioBlock, &WS2812B_program);
    uint bits = (b4 == NONE ? 24 : 32);
    WS2812B_program_init(pioBlock, stateMachine, offset, dataPin, 800000, bits);
}

}