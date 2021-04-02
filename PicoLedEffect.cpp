#include "PicoLedEffect.hpp"

namespace PicoLed {

PicoLedEffect::PicoLedEffect(PicoLedController &controller): controller(controller) {
    this->lastUpdate = to_ms_since_boot( get_absolute_time() );
}

PicoLedEffect::~PicoLedEffect() {

}

bool PicoLedEffect::animate() {
    uint32_t timeNow = to_ms_since_boot( get_absolute_time() );
    uint32_t timeGone = timeNow - lastUpdate;
    if (update(timeGone, timeNow)) {
        lastUpdate = timeNow;
        return true;
    }
    return false;
}

void PicoLedEffect::reset() {
    lastUpdate = to_ms_since_boot( get_absolute_time() );
}

bool PicoLedEffect::update(uint32_t timeGone, uint32_t timeNow) {
    return false;    
}

}