#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <functional>
#include "Lights.h"
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// wifi credentials
const char* ssid = "your wifi network name";
const char* password = "your wifi password";

// callbacks
void turnOnLight();
void turnOnLightBlue();
void turnOnLightRed();
void turnOnLightGreen();
void turnOnLightPurple();
void turnOnLight50();
void turnOffLight();

// Define your switches here. Max 14
// Format: Alexa invocation name, local port no, on callback, off callback
#define NUMSWITCHES  5
Switch *allSwitches[] = {
                          new Switch("counter", 80, turnOnLight, turnOffLight), 
                          new Switch("blue counter", 81, turnOnLightBlue, turnOffLight),
                          new Switch("red counter", 82, turnOnLightRed, turnOffLight),
                          new Switch("green counter", 83, turnOnLightGreen, turnOffLight),
                          new Switch("purple counter", 84, turnOnLightPurple, turnOffLight)
                        };
                        
// The pin on the ESP8266 that is connected to the NeoPixels
#define PIN            D3

// Number of NeoPixels are attached to the ESP8266
#define NUMPIXELS      300

// Global variables
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Lights lights(&pixels);
boolean wifiConnected = false;
UpnpBroadcastResponder upnpBroadcastResponder;

// prototypes
boolean connectWifi();

void setup() {
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
  Serial.begin(9600);
  
  pixels.begin();
  pixels.show();
  pixels.setBrightness(255);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
   
    Serial.println("Adding switches upnp broadcast responder");
    for(int i = 0; i < NUMSWITCHES; i++){
      upnpBroadcastResponder.addDevice(*allSwitches[i]);
    }
  }  
}

void loop() {
  ESP.wdtFeed();

  if(wifiConnected){
    upnpBroadcastResponder.serverLoop();
    for(int i = 0; i < NUMSWITCHES; i++){
      allSwitches[i]->serverLoop();
    } 
  }
}
      
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void turnOnLightBlue() {
  lights.showColor(0, 0, 255);
}

void turnOnLightRed(){
  lights.showColor(255, 0, 0);
}

void turnOnLightGreen(){
  lights.showColor(0, 255, 0);
}

void turnOnLightPurple(){
  lights.showColor(255, 0, 255);
}

void turnOnLight50(){
   lights.setBrightness(50);
}

void turnOnLight() {
  lights.showRainbow();
}

void turnOffLight() {
  lights.off();
}
