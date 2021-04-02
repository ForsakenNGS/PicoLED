# PicoLED library

## About

PicoLED is a library to interface with Multi-Pixel LED devices.
It is written for WS2812B LEDs, but has the option to implement other types.

PIO state machines are used for communication, like use in the official example, 
which limits the overall count of data lines to 8.

## Installation

- Download the files from the repository and extract them as e.g. a "PicoLED" folder.
- Copy or symlink the folder into your Project.
- Include the libraries in your CMAKE file e.g.: `include("PicoLed/PicoLed.cmake")`

## Example

In the Repository https://github.com/ForsakenNGS/PicoLedExample I supply an example application that will go trough a few test patterns followed by a loop of the example animations.

## API

### Initialisation

```
PicoLed::addLeds<PicoLedTarget>(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataByte b1, DataByte b2, DataByte b3, DataByte b4);

PicoLed::addLeds<PicoLedTarget>(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, DataFormat format);
```

### Parameters:

- **pioBlock** The PIO block of the pico that should be used. (Either `pio0` or `pio1`)
- **stateMachine** Index of the state machine used for communation. (Each pio block has 4 state machines from `0-3`)
- **dataPin** The pin that is used for communication. (Any of the GPIO-Pins afaik)
- **numLeds** The number of LEDs in the strip attached to the given data pin. (Can be virtualized into part-strips for daisy chained elements)
- **b1-b4** For the manual byte formats. Supply 
    `NONE`, `RED`, `GREEN`, `BLUE` or `WHITE` in the order that your LED Strip expects it. (See the enum `PioLed::DataByte` in the file `PicoLedTarget.hpp`)
- **format** One of the predefined formats. Currently: `FORMAT_RGB`, `FORMAT_GRB` or `FORMAT_WRGB` (See the enum `PioLed::DataFormat` in the file `PicoLedTarget.hpp`)

### Example:
```
// 0. Initialize LED strip
auto ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
```

### High-Level fuctions
See also `PicoLedController.hpp`
```
uint getNumLeds();
uint8_t getBrightness();
void setBrightness(uint8_t brightness);
DrawMode getDrawMode();
void setDrawMode(DrawMode mode);
Color getPixelColor(uint index);
void setPixelColor(uint index, Color color);
void setPixelColor(uint index, Color color, DrawMode mode);
void show();
void clear();
void clear(Color color);
void fill(Color color);
void fill(Color color, uint first);
void fill(Color color, uint first, uint count);
void fillGradient(Color colorStart, Color colorEnd);
void fillGradient(Color colorStart, Color colorEnd, uint first);
void fillGradient(Color colorStart, Color colorEnd, uint first, uint count);
void fillRainbow(uint8_t initialHue, uint8_t deltaHue);
void fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first);
void fillRainbow(uint8_t initialHue, uint8_t deltaHue, uint first, uint count);
void fade(Color color, double factor);
void fade(Color color, uint first, double factor);
void fade(Color color, uint first, uint count, double factor);
void fadeLine(Color color, double first, double factor);
void fadeLine(Color color, double first, double count, double factor);
void fadePixel(uint index, Color color, double factor);
void fadeValue(Color color, uint8_t value);
void fadeValue(Color color, uint first, uint8_t value);
void fadeValue(Color color, uint first, uint count, uint8_t value);
void fadePixelValue(uint index, Color color, uint8_t value);
PicoLedController slice(uint start, uint end);
```
**getNumLeds** Get the number of leds as supplied in the initialisation.

**getBrightness** Get the current overall brightness. By default `255` / maximum brightness.

**setBrightness** Change the overall brightness. Anything between `0` and `255`. This will reduce the resolution of the colors!

**getDrawMode** The current draw mode used for high-level draw functions. (One of `MODE_SET`, `MODE_ADD` or `MODE_SUB`, By default `MODE_SET`)

**setDrawMode** Change the draw mode used for high-level draw functions. (One of `MODE_SET`, `MODE_ADD` or `MODE_SUB`, By default `MODE_SET`)

**getPixelColor** Gets the current color of the pixel at the given `index`.

**setPixelColor** Changes the pixel `index` to the given `color`.

**show** Transmit the changes to the LED controller(s) at the data pin. Must be called for the pixel changes to have any effect.

**clear** Reset all pixels to `color`. (Black/off if no color is given)

**fill** Set the color of `count` pixels starting at the `first` index to the given `color`.

**fillGradient** Set the color of `count` pixels starting at the `first` index to a gradient from `colorStart` to `colorEnd`.

**fillRainbow** Set the color of `count` pixels starting at the `first` index to a color gradient start at the `initialHue` changing it by `deltaHue` per pixel.

**fade** Fade the color of `count` pixels starting at the `first` index to the given `color` by the given `factor`. (`1.0` replaces the color completely, `0.0` does nothing)

**fadeLine** Fade the color of `count` pixels starting at the `first` index to the given `color` by the given `factor`. Will interpolate pixels at the borders of the given line, allowing for smoother sub-pixel movements.

**fadePixel** Fade the color of the `index` pixel to the given `color` by the given `factor`.

**fadeValue** Fade the color of `count` pixels starting at the `first` index to the given `color` by the up to the absolute `value` supplied. (`255` will change it all the way `0` not at all for 50% grey `128` will change it all the way)

**fadePixelValue** Fade the color of `count` pixels starting at the `first` index to the given `color` by the up to the absolute `value` supplied. (`255` will change it all the way `0` not at all for 50% grey `128` will change it all the way)

**slice** Returns a virtual LED strip from the `start` to the `end` pixel to control those seperately. (`show` will still transmit the whole strip)

### Low-Level functions
See also `PicoLedTarget.hpp`
```
virtual uint getNumLeds();
virtual uint8_t getBrightness();
virtual void setBrightness(uint8_t brightness);
virtual uint32_t getData(uint index);
virtual void setData(uint index, uint32_t value);
virtual void show();
Color getPixelColor(uint index);
void setPixelColor(uint index, Color color);
void fill(Color color, uint first, uint count);
```

**getNumLeds** Get the number of leds as supplied in the initialisation.

**getBrightness** Get the current overall brightness. By default `255` / maximum brightness.

**setBrightness** Change the overall brightness. Anything between `0` and `255`. This will reduce the resolution of the colors!

**getData** Read the raw pixel data as it will be sent to the LED controller. (Format depends on the format supplied on the initialisation)

**setData** Set the raw pixel data for the pixel at `index`. (Format depends on the format supplied on the initialisation)

**show** Transmit the changes to the LED controller(s) at the data pin. Must be called for the pixel changes to have any effect.

**getPixelColor** Gets the current color of the pixel at the given `index`.

**setPixelColor** Changes the pixel `index` to the given `color`.

**fill** Changes `count` pixels starting at the `first` index to the given `color`.

## Effects
The library includes the `PicoLedEffect` class that provides a base for implementing time-based effects. It includes a few example implementations: `Marquee`, `Particles`, `Stars` `Comet`, `Bounce`