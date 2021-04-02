#include "Stars.hpp"
#include "PicoLed.hpp"

namespace PicoLed {

    Stars::Stars(PicoLedController &controller, Color color, double spawnRate):
    Fade(controller, RGB(0, 0, 0), 0.5), palette(), spawnRate(spawnRate), nextStar(0)
    {
        palette.push_back(color);
    }

    Stars::Stars(PicoLedController &controller, Color color, double spawnRate, double fadeRate): 
    Fade(controller, RGB(0, 0, 0), fadeRate), palette(), spawnRate(spawnRate), nextStar(0)
    {
        palette.push_back(color);
    }

    Stars::Stars(PicoLedController &controller, vector<Color> palette, double spawnRate): 
    Fade(controller, RGB(0, 0, 0), 0.5), palette(palette), spawnRate(spawnRate), nextStar(0)
    {
    }

    Stars::Stars(PicoLedController &controller, vector<Color> palette, double spawnRate, double fadeRate): 
    Fade(controller, RGB(0, 0, 0), fadeRate), palette(palette), spawnRate(spawnRate), nextStar(0)
    {
    }
    
    bool Stars::update(uint32_t timeGone, uint32_t timeNow) {
        fadePixels(timeNow);
        if (nextStar < timeGone) {
            nextStar = (uint32_t)(1000.0 / spawnRate);
            uint ledIndex = random() % controller.getNumLeds();
            uint colorIndex = random() % palette.size();
            controller.setPixelColor(ledIndex, palette[colorIndex]);
        } else {
            nextStar -= timeGone;
        }
        return true;
    }

}