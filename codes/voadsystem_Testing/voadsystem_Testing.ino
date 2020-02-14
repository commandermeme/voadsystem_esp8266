#include <ArduinoJson.h>
#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "memz";
const char* password = "Mypreciousone@";

TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  display.init();
  display.flipScreenVertically();
  
  while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting. . .");

      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, "Connecting....");
      display.display();
      display.clear();
  }

  Serial.println("Connected!");
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Connected!");
  display.display();
  display.clear();
}

void loop() {
  while(Serial.available()) {
    if(gps.encode(Serial.read())) {
      String latitude = String(gps.location.lat(), 6);
      String longitude = String(gps.location.lng(), 6);
      String current_speed = String(gps.speed.kmph(), 1);
  
      HTTPClient http;
  //  http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=10.297459&longitude=123.896567&speed=20&system=voad1");
      http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
      int httpCode = http.GET();
      Serial.println(httpCode);
      Serial.println(http.getString());
      
      if(httpCode > 200) {
        Serial.println(httpCode);
        Serial.println(http.getString());
       
        StaticJsonDocument<200> doc;
        String data = http.getString();

//        DeserializationError deserializeJson(doc, data);
        DeserializationError error = deserializeJson(doc, data);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
//          return;
        }
      
        const char* street = doc["street"];
        int current_speed_1 = doc["speed"];
        
        Serial.println("Street: "+ String(street)); 
        Serial.println("Latitude: "+ String(current_speed_1));
    
        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 0, String(millis()));
        display.drawString(0, 10, String(current_speed_1));
        display.drawString(0, 20, latitude);
        display.drawString(0, 30, longitude);
        display.drawString(0, 40, current_speed);
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
        display.display();
        display.clear();
      }
      else {
        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 0, String(millis()));
        display.drawString(0, 10, "Error!");
        display.drawString(0, 20, latitude);
        display.drawString(0, 30, longitude);
        display.drawString(0, 40, current_speed);
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
        display.display();
        display.clear();
      }
      http.end();
      delay(500);
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
