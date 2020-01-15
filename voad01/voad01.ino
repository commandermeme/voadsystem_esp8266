#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson-v5.13.5.h>
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
  }

  Serial.println("Connected!");
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
    api();
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

void api() {
  if(WiFi.status() == WL_CONNECTED) {

      String latitude = String(gps.location.lat(), 6);
      String longitude = String(gps.location.lng(), 6);
      String current_speed = String(gps.speed.kmph(), 0);

      HTTPClient http; //Object of class HTTPClient
      http.begin("http://192.168.1.2/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
      int httpCode = http.GET();
  
      if (httpCode > 0) {
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& data = jsonBuffer.parseObject(http.getString());
   
        const char* street = data["street"]; 
        const char* speed_limit = data["speed_limit"];
        const char* speed = data["speed"]; 
        const char* violation = data["violation"];  
  
        Serial.print("Street: ");
        Serial.println(street);
        Serial.print("Speed Limit: ");
        Serial.println(speed_limit);
        Serial.print("Speed: ");
        Serial.println(speed);
        Serial.print("Violation: ");
        Serial.println(violation);

        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 0, String(millis()));
        display.drawString(0, 10, String(street));
        display.drawString(0, 20, String(speed_limit));
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.setFont(ArialMT_Plain_24);
        display.drawString(64, 40, String(gps.speed.kmph(), 0));
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.setFont(ArialMT_Plain_16);
        display.drawString(128, 45, "km/h");
      }
      http.end(); //Close connection
    }
    delay(1000);
}
