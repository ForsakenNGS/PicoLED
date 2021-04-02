#include "Bounce.hpp"
#include "PicoLed.hpp"

namespace PicoLed {

    Bounce::Bounce(PicoLedController &controller, double fadeRate, double gravity):
    Fade(controller, RGB(0, 0, 0), fadeRate), gravity(gravity)
    {
    }

    void Bounce::addBall(Color color, double length) {
        uint numLeds = controller.getNumLeds();
        balls.push_back((struct BounceBall){
            .color = color, .length = length, .offset = 0.0, 
            .speed = (double)numLeds * 0.5 + (double)(random() % 100) / 10.0
        });
    }

    void Bounce::reset() {
        uint numLeds = controller.getNumLeds();
        // Iterate balls
        for (std::vector<BounceBall>::iterator it = balls.begin() ; it != balls.end(); ++it) {
            // Reset offsets and speeds
            it->offset = 0.0;
            it->speed = (double)numLeds * 0.5 - (double)(random() % 100) / 10.0;
        }
        // Reset update
        Fade::reset();
    }

    bool Bounce::update(uint32_t timeGone, uint32_t timeNow) {
        uint numLeds = controller.getNumLeds();
        // Fade tail
        fadePixels(timeNow);
        // Draw balls
        DrawMode prevMode = controller.getDrawMode();
        controller.setDrawMode(MODE_ADD);
        // Iterate balls
        for (std::vector<BounceBall>::iterator it = balls.begin() ; it != balls.end(); ++it) {
            // Draw body
            controller.fadeLine(it->color, it->offset, it->length, 1.0);
            // Calculate movement
            it->offset += (double)timeGone * it->speed / 1000.0;
            it->speed -= (double)timeGone / 1000.0 * gravity;
            if (it->offset < 0.0) {
                it->offset *= -1.0;
                it->speed *= -0.9;
                if (abs(it->speed) < 5.0) {
                    // Reset offsets and speeds
                    it->offset = 0.0;
                    it->speed = (double)numLeds * 0.5 + (double)(random() % 100) / 10.0;
                }
            }
        }
        controller.setDrawMode(prevMode);
        return true;
    }

}