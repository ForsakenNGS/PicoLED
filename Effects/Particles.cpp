#include "Particles.hpp"
#include "PicoLed.hpp"

using std::min, std::max;

namespace PicoLed {

Particles::Particles(PicoLedController &controller, vector<Color> palette):
Fade(controller, RGB(0, 0, 0), 1.0), palette(palette), sources(), spreadFactor(1.0)
{
    heat = new uint8_t[controller.getNumLeds()]{ 0 };
}

Particles::Particles(PicoLedController &controller, vector<Color> palette, double spreadFactor):
Fade(controller, RGB(0, 0, 0), 1.0), palette(palette), sources(), spreadFactor(spreadFactor)
{
    heat = new uint8_t[controller.getNumLeds()]{ 0 };
}

Particles::Particles(PicoLedController &controller, vector<Color> palette, double spreadFactor, double coolingRate):
Fade(controller, RGB(0, 0, 0), coolingRate), palette(palette), sources(), spreadFactor(spreadFactor)
{
    heat = new uint8_t[controller.getNumLeds()]{ 0 };
}

Particles::~Particles() {
    delete heat;
}

void Particles::addSource(uint pixelIndex, double spawnRate) {
    addSource(pixelIndex, spawnRate, 1.0);
}

void Particles::addSource(uint pixelIndex, double spawnRate, double spawnSpeed) {
    sources.push_back(ParticleSource{
        .pixelIndex = pixelIndex, .spawnRate = 0.1 / spawnRate, .spawnValue = 0.0, 
        .spawnSpeed = spawnSpeed * controller.getNumLeds() / 10.0
    });
}

void Particles::addParticle(double offset, double speed, uint8_t heat) {
    particles.push_back(Particle{
        .offset = offset, .speed = speed, .heat = heat
    });
}

uint8_t Particles::getHeatSpread(uint index) {
    return (
        (uint)(index > 1 ? heat[index-2] : heat[index]) +
        (uint)(index > 0 ? heat[index-1] : heat[index]) +
        (uint)heat[index] +
        (uint)(index < controller.getNumLeds()-1 ? heat[index+1] : heat[index]) +
        (uint)(index < controller.getNumLeds()-2 ? heat[index+2] : heat[index])
    ) / 5;
}

Color Particles::getHeatColor(uint8_t value) {
    if (palette.size() <= 1) {
        return palette[0];
    }
    double segmentLength = 256.0 / (palette.size() - 1);
    uint8_t colorIndex = floor(value / segmentLength);
    return MixColors(palette[colorIndex+1], palette[colorIndex], (double)(value - colorIndex * segmentLength) / segmentLength);
}

bool Particles::fadePixels(uint32_t timeNow) {
    uint32_t timeGone = timeNow - fadeLastTick;
    uint32_t fadeInterval = max<uint32_t>(1, 1000.0 / 255.0 / fadeRate);
    uint8_t fadeValue = timeGone / fadeInterval;
    if (fadeValue > 1) {
        auto numLeds = controller.getNumLeds();
        // Cool random pixels
        for (uint i = 0; i < numLeds; i++) {
            if (rand() % 2) {
                heat[i] -= min<uint8_t>(heat[i], fadeValue);
            }
        }
        // Spread the heat
        uint heatNew[numLeds] { 0 };
        double heatAvg;
        double spreadAmount = spreadFactor * numLeds / 15.0;
        // Spread forward
        heatAvg = heat[0];
        heatNew[0] += heat[0];
        for (uint i = 1; i < numLeds; i++) {
            heatAvg = (heatAvg * spreadAmount + heat[i]) / (1.0 + spreadAmount);
            heatNew[i] += heatAvg;
        }
        // Spread backward
        heatAvg = heat[numLeds-1];
        heatNew[numLeds-1] += heat[numLeds-1];
        for (uint i = numLeds-2; i-- > 0; ) {
            heatAvg = (heatAvg * spreadAmount + heat[i]) / (1.0 + spreadAmount);
            heatNew[i] += heatAvg;
        }
        // Apply new values
        for (uint i = 0; i < numLeds; i++) {
            heat[i] = (heatNew[i] * fadeValue / 2 + heat[i] * 2) / (fadeValue + 2);
        }
        // Update particles
        for (std::vector<Particle>::iterator it = particles.begin() ; it != particles.end(); ++it) {
            // Apply heat
            uint pixelLeft = floor(it->offset);
            uint pixelRight = ceil(it->offset);
            if ((pixelLeft >= 0) && (pixelLeft < numLeds)) {
                double pixelRatio = it->offset - pixelLeft;
                uint8_t heatChange = fadeValue * it->heat * pixelRatio;
                heat[pixelLeft] += min<uint8_t>(255 - heat[pixelLeft], heatChange);
            }
            if ((pixelLeft != pixelRight) && (pixelRight >= 0) && (pixelRight < numLeds)) {
                double pixelRatio = pixelRight - it->offset;
                uint8_t heatChange = fadeValue * it->heat * pixelRatio;
                heat[pixelRight] += min<uint8_t>(255 - heat[pixelRight], heatChange);
            }
            // Update particle position and heat
            it->offset += it->speed * fadeValue / numLeds;
            it->heat -= min<uint8_t>(it->heat, fadeValue);
            if ((it->heat == 0) || (it->offset < 0.0) || (it->offset > numLeds)) {
                // Particle lost all heat or went out of bounds
                particles.erase(it--);
            }
        }
        fadeLastTick = timeNow - (timeGone % fadeInterval);
        return true;
    } else {
        return false;
    }
}

bool Particles::update(uint32_t timeGone, uint32_t timeNow) {
    bool updated = false;
    // Fade heat
    if (fadePixels(timeNow)) {
        updated = true;
    }
    // Iterate sources and potentially create particles
    for (std::vector<ParticleSource>::iterator it = sources.begin() ; it != sources.end(); ++it) {
        it->spawnValue += (double)(random() % 1000) / 1000.0 * (double)timeGone / 1000.0;
        while (it->spawnValue > it->spawnRate) {
            it->spawnValue -= it->spawnRate;
            addParticle(it->pixelIndex, it->spawnSpeed * (rand() % 50 + 50) / 100.0, rand() % 127 + 128);
        }
    }
    // Draw current state
    if (updated) {
        for (uint i = 0; i < controller.getNumLeds(); i++) {
            controller.setPixelColor(i, getHeatColor(heat[i]));
        }
    }
    return updated;
}

}