#include <ArduinoJson.h>
#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Gourgeous";
const char* password = "Mypreciousone";
String latitude = "0";
String longitude = "0";
String current_speed = "0";

TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);

void setup() {
  Serial.begin(9600);
  
  display.init();
  display.flipScreenVertically();

  while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting. . .");
  }

  Serial.println("Connected!");
}

void loop() {
    
  
  
  while(Serial.available() > 0)
//  gps.encode(Serial.read());
    gps.encode(Serial.read());
        latitude = String(gps.location.lat(), 6);
        longitude = String(gps.location.lng(), 6);
        current_speed = int(gps.speed.kmph());
      

      if((WiFi.status() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
        int httpCode = http.GET();
        Serial.println(httpCode);
//        Serial.println(http.getString());
        Serial.println(current_speed);
  
        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 10, String(httpCode));
        display.drawString(0, 0, String(millis()));      
        display.drawString(0, 20, String(latitude));
        display.drawString(0, 30, String(longitude));
        display.drawString(0, 40, String(current_speed));
        display.display();
        display.clear();
        http.end();
      }
      delay(1000);
//    }
//      display.clear();
//      displayGps();
//      display.display();
//      display.clear();
      
      
//      Serial.println(millis());
      
      
      

      
    
//  }
//  delay(100);

//  getLatitude();
//  delay(1000);
}

void displayGps() {
//  if(gps.location.isUpdated()) {
//    display.setFont(ArialMT_Plain_10);
//    display.setTextAlignment(TEXT_ALIGN_RIGHT);
//    display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
//    display.drawString(0, 0, String(millis()));
//    display.drawString(0, 10, "Lat: " + String(gps.location.lat(), 6));
//    display.drawString(0, 20, "Lng: " + String(gps.location.lng(), 6));
//    display.setTextAlignment(TEXT_ALIGN_CENTER);
//    display.setFont(ArialMT_Plain_24);
//    display.drawString(64, 40, String(gps.speed.kmph(), 0));
//    display.setTextAlignment(TEXT_ALIGN_RIGHT);
//    display.setFont(ArialMT_Plain_16);
//    display.drawString(128, 45, "km/h");
//
//    Serial.println(String(gps.location.lat(), 6));

//    String latitude = getLatitude();

    HTTPClient http;
//    http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +String(gps.location.lat(), 6)+ "&longitude=" +String(gps.location.lng(), 6)+ "&speed=" +String(gps.speed.kmph(), 0)+ "&system=voad001");
//    http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
//    http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=10.297392&longitude=123.896387&speed=19&system=voad001");
    int httpCode = http.GET();
    Serial.println(httpCode);
    Serial.println(http.getString());

    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 30, "HttpCode: "+ String(httpCode));

    http.end();
    display.display();
    display.clear();
//  else {
//    display.setFont(ArialMT_Plain_10);
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
//    display.drawString(0, 0, String(millis()));
//    Serial.println(millis());
//    
//    display.setTextAlignment(TEXT_ALIGN_CENTER);
//    display.setFont(ArialMT_Plain_16);
//    display.drawString(64, 0, "NO");
//    display.drawString(64, 20, "SATELLITES");
//    display.drawString(64, 40, "CONNECTION");
//  }
}
