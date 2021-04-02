#ifndef VIRTUALSTRIP_H
#define VIRTUALSTRIP_H

#include <memory>
#include "PicoLedTarget.hpp"

using std::shared_ptr;

namespace PicoLed {

class VirtualStrip: public PicoLedTarget {

    public:
        VirtualStrip(shared_ptr<PicoLedTarget> target, uint start, uint end);
        ~VirtualStrip();

        uint8_t getBrightness();
        void setBrightness(uint8_t brightness);
        uint32_t getData(uint index);
        void setData(uint index, uint32_t value);

        void show();

    protected:
        shared_ptr<PicoLedTarget> target;
        uint start;
        uint end;

        uint getRealIndex(uint index);
};

}

#endif