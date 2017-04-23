#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <functional>
#include "Lights.h"
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

// callbacks
void turnOnWindowBlue();
void turnOnWindow();
void turnOffWindow();

// wifi credentials
const char* ssid = "your wifi network name";
const char* password = "your wifi password";

// The pin on the ESP8266 that is connected to the NeoPixels
#define PIN            D3

// Number of NeoPixels are attached to the ESP8266
#define NUMPIXELS      300

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Lights lights(&pixels);
boolean wifiConnected = false;
UpnpBroadcastResponder upnpBroadcastResponder;

Switch *window = NULL;
Switch *blueWindow = NULL;

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
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    window = new Switch("window", 80, turnOnWindow, turnOffWindow);
    blueWindow = new Switch("blue window", 81, turnOnWindowBlue, turnOffWindow);
    
    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*window);
    upnpBroadcastResponder.addDevice(*blueWindow);
  }  
}

void loop() {
  ESP.wdtFeed();

  if(wifiConnected){
    upnpBroadcastResponder.serverLoop();
    
    window->serverLoop();
    blueWindow->serverLoop();
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

void turnOnWindowBlue() {
  lights.blueMode();
}

void turnOnWindow() {
  lights.rainbowMode();
}

void turnOffWindow() {
  lights.offMode();
}
