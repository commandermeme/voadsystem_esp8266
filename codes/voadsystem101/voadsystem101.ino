#include <ArduinoJson-v5.13.5.h>
#include <OLEDDisplayUi.h>
#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);

//const char* ssid = "";

void setup() {
  Serial.begin(9600);
  
  display.init();
  display.flipScreenVertically();

  headerText();
}

unsigned long updateScreen = 0;

void loop() {
  unsigned long now = millis();
  
  while(Serial.available() > 0) {
    if(gps.encode(Serial.read())) {
      if(now - updateScreen > 100) {
        displayGps();
        updateScreen = now; 
      }
    }
  }
}

void headerText() {
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "VEHICLE");
  display.drawString(64, 15, "OVERSPEEDING");
  display.drawString(64, 30, "& DETECTION");
   display.drawString(64, 45, "SYSTEM");
  display.display();
  delay(5000);
  display.clear();
}

void displayGps() {
  if(gps.location.isValid()) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  
//    display.drawString(0, 10, "Lat: " + String(gps.location.lat(), 5));
//    display.drawString(0, 20, "Lng: " + String(gps.location.lng(), 5));
    display.drawString(0, 0, "Aznar Road");
    display.drawString(0, 10, "Lat: " + String(gps.location.lat(), 6));
    display.drawString(0, 20, "Lng: " + String(gps.location.lng(), 6));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 10, "Sat: " + String(gps.satellites.value()));
  
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 30, String(gps.speed.kmph(), 1));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(128, 50, "km/h");
    
//    display.setFont(ArialMT_Plain_10);
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
//    display.drawString(0, 54, String(gps.time.hour())+ ":" + String(gps.time.minute())+ ":" + String(gps.time.second()));
//    display.setTextAlignment(TEXT_ALIGN_RIGHT);
//    display.drawString(128, 54, String(gps.date.month())+ ":" + String(gps.date.day())+ ":" + String(gps.date.year()));
    
//    display.drawString(0, 40, "Time: " + String(gps.time.hour())+ ":" + String(gps.time.minute())+ ":" + String(gps.time.second()));
//    display.drawString(0, 50, "Date: " + String(gps.date.month())+ ":" + String(gps.date.day())+ ":" + String(gps.date.year()));
    
    display.display();
    display.clear();
  }
  else {
    Serial.println("Invalid");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(128, 0, String(millis()));
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, "NO");
    display.drawString(64, 20, "SATELLITES");
    display.drawString(64, 40, "CONNECTION");
    display.display();
    display.clear(); 
  }
}
