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

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting. . .");
  }
  Serial.println("Connected!");
}

void loop() {
  while(Serial.available()) {
    if(gps.encode(Serial.read())) {
      String latitude = String(gps.location.lat(), 6);
      String longitude = String(gps.location.lng(), 6);
      String current_speed = String(gps.speed.kmph(), 1);
      
//      if(WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
        int httpCode = http.GET();
        Serial.println(httpCode);
        Serial.println(http.getString());

        http.end();
//      }
      delay(1000);
    }
  }
}
