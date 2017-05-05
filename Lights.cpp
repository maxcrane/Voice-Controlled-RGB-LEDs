#include "Arduino.h"
#include "Lights.h"
#include <Adafruit_NeoPixel.h>

Lights::Lights(Adafruit_NeoPixel* pixels)
{
  _pixels = pixels;
  _numPixels = pixels->numPixels();
}

void Lights::showColor(int r, int g, int b){
  for(int i=0; i<_numPixels; i++){
    _pixels->setPixelColor(i, _pixels->Color(r, g, b)); 
    _pixels->show(); 
    ESP.wdtFeed();
  } 
}

void Lights::setBrightness(int brightness){
  _pixels->setBrightness(brightness/256);
}
    

void Lights::off()
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

