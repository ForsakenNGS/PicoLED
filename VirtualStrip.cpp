#include "VirtualStrip.hpp"

namespace PicoLed {

VirtualStrip::VirtualStrip(shared_ptr<PicoLedTarget> target, uint start, uint end):
PicoLedTarget(*target), target(target), start(start), end(end)
{
    this->numLeds = (start < end ? end - start : start - end);
}

VirtualStrip::~VirtualStrip() {
}

uint8_t VirtualStrip::getBrightness() {
    return target->getBrightness();
}

void VirtualStrip::setBrightness(uint8_t brightness) {
    target->setBrightness(brightness);
}

uint32_t VirtualStrip::getData(uint index) {
    return target->getData( getRealIndex(index) );
}

void VirtualStrip::setData(uint index, uint32_t value) {
    target->setData( getRealIndex(index), value );
}

void VirtualStrip::show() {
    target->show();
}

uint VirtualStrip::getRealIndex(uint index) {
    return (start < end ? start + index : start - index);
}

}