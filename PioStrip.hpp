#ifndef PIOSTRIP_H
#define PIOSTRIP_H

#include "hardware/pio.h"
#include "PicoLedTarget.hpp"

namespace PicoLed {

class PioStrip: public PicoLedTarget {
    public:
        PioStrip(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        virtual ~PioStrip();

        uint32_t getData(uint index);
        void setData(uint index, uint32_t value);
        void show();
    protected:
        PIO pioBlock;
        uint stateMachine;
        uint dataPin;
        uint32_t *data;
};

};

#endif