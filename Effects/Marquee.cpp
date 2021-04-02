#include "Marquee.hpp"
#include "PicoLed.hpp"

namespace PicoLed {

    Marquee::Marquee(PicoLedController &controller, vector<Color> palette, double length, double speed): 
    PicoLedEffect(controller), palette(palette), length(length), spacing(0.0), speed(speed), offset(0.0)
    {
    }

    Marquee::Marquee(PicoLedController &controller, vector<Color> palette, double length, double speed, double spacing): 
    PicoLedEffect(controller), palette(palette), length(length), spacing(spacing), speed(speed), offset(0.0)
    {
    }
    
    bool Marquee::update(uint32_t timeGone, uint32_t timeNow) {
        uint numLeds = controller.getNumLeds();
        double segmentLength = (length + spacing);
        double effectLength = (double)palette.size() * segmentLength;
        // Draw current state
        double segmentStart = 0.0 - std::fmod(offset, segmentLength);
        uint paletteIndex = floor(offset / segmentLength);
        controller.clear();
        while (segmentStart < numLeds) {
            controller.fadeLine(palette[paletteIndex], segmentStart, length, 1.0);
            segmentStart += segmentLength;  
            paletteIndex = ++paletteIndex % palette.size();
        }
        // Calculate movement
        offset += (double)timeGone * speed / 1000.0;
        if (offset > effectLength) {
            offset = std::fmod(offset, effectLength);
        } else if (offset < 0.0) {
            offset = effectLength + offset;
        }
        return true;
    }

}