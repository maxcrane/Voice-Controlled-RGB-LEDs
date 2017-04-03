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
    void offMode();
    void rainbowMode();
    void show();
    
    
  private:
    Adafruit_NeoPixel* _pixels;
    int _numPixels;
    String _lightsMode;
    void showNothing();
    void showRainbow();
};

#endif
