/*
  Lights.h - for adafruit lights
*/
#ifndef Lights_h
#define Lights_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Lights
{
  public:
    Lights(Adafruit_NeoPixel* pixels);
    void off();
    void showRainbow();
    void showColor(int r, int g, int b);
    void setBrightness(int brightness);
    
  private:
    Adafruit_NeoPixel* _pixels;
    int _numPixels;
};

#endif
