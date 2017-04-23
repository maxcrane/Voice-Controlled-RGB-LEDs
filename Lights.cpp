#include "Arduino.h"
#include "Lights.h"
#include <Adafruit_NeoPixel.h>

Lights::Lights(Adafruit_NeoPixel* pixels)
{
  _pixels = pixels;
  _numPixels = pixels->numPixels();
  _lightsMode = "off";
}

void Lights::offMode()
{
  _lightsMode = "off";
  show();
}

void Lights::rainbowMode()
{
  _lightsMode = "rainbow";
  show();
}

void Lights::blueMode()
{
  _lightsMode = "blue";
  show();
}


void Lights::show()
{
  if(_lightsMode.equals("rainbow")){
    showRainbow();
  } else if(_lightsMode.equals("blue")){
    showBlue();
  }
  else {
    showNothing();
  }
}

void Lights::showBlue()
{
  
  for(int i=0; i<_numPixels; i++){
    _pixels->setPixelColor(i, _pixels->Color(0,0,255)); // blue
    _pixels->show(); // This sends the updated pixel color to the hardware.
    ESP.wdtFeed();
  } 
}

void Lights::showNothing()
{
  
  for(int i=0; i<_numPixels; i++){
    _pixels->setPixelColor(i, _pixels->Color(0,0,0)); // No color
    _pixels->show(); // This sends the updated pixel color to the hardware.
    ESP.wdtFeed();
  } 
}

void Lights::showRainbow(){
  int j = (millis() >> 4) & 255;

  for(int i=0;i<_numPixels;i++){
      int ij = 255 - ((i + j) & 255);
      if(ij < 85){
        _pixels->setPixelColor(i, _pixels->Color(255 - ij * 3, 0, ij * 3));
      }
      else if(ij < 170)
        _pixels->setPixelColor(i, _pixels->Color(0, (ij - 85) * 3, 255 - (ij - 85) * 3));
      else
        _pixels->setPixelColor(i, (ij -170) * 3, 255 - (ij -170) * 3, 0);
    
    _pixels->show(); // This sends the updated pixel color to the hardware.
    ESP.wdtFeed();
  }
 
}

