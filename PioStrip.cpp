#include "PicoLed.hpp"
#include "PioStrip.hpp"
#include <stdio.h>

namespace PicoLed {

PioStrip::PioStrip(
    PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4
):
PicoLedTarget(numLeds, b1, b2, b3, b4), pioBlock(pioBlock), stateMachine(stateMachine), dataPin(dataPin)
{
    data = new uint32_t[numLeds];
    fill(RGB(0, 0, 0), 0, numLeds);
}

PioStrip::~PioStrip() {
    delete data;
}

uint32_t PioStrip::getData(uint index) {
    return data[index];
}

void PioStrip::setData(uint index, uint32_t value) {
    data[index] = value;
}

void PioStrip::show() {
    for (uint i = 0; i < numLeds; i++) {
        pio_sm_put_blocking(pioBlock, stateMachine, scalePixelData(data[i], brightness));
    }
}

}