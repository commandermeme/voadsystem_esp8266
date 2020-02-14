#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "memz";
const char* password = "Mypreciousone@";

TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);

void setup() {
  Serial.begin(9600);
  
  display.init();
  display.flipScreenVertically();
}

void loop() {
  while(Serial.available()) {
    if(gps.encode(Serial.read())) {
      display.clear();
      displayGps();
      display.display();
    }
  }
}

void displayGps() {
  if(gps.location.isValid()) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, String(millis()));
    display.drawString(0, 10, "Lat: " + String(gps.location.lat(), 6));
    display.drawString(0, 20, "Lng: " + String(gps.location.lng(), 6));
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 40, String(gps.speed.kmph(), 0));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(128, 45, "km/h");
  }
  else {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, String(millis()));
    Serial.println(millis());
    
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, "NO");
    display.drawString(64, 20, "SATELLITES");
    display.drawString(64, 40, "CONNECTION");
  }
}
