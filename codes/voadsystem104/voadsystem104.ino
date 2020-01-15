#include <SH1106Wire.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"

const char* ssid = "Gourgeous";
const char* password = "Mypreciousone";

TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);
ESP8266WebServer server(80);

void handleRoot() {
  String page = MAIN_page;
  server.send(200, "text/html", page);
}

void handleLatitude() {
  double lat = gps.location.lat();
  String Lat_value = String(lat, 5);

  server.send(200, "text/plane", Lat_value);
}

void handleLongitude() {
  double lng = gps.location.lng();
  String Lng_value = String(lng, 5);

  server.send(200, "text/plane", Lng_value);
}

void handleSpeed() {
  double speed = gps.speed.kmph();
  String Speed_value = String(speed, 0);

  server.send(200, "text/plane", Speed_value);
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("Successfully connected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/readLatitude", handleLatitude);
  server.on("/readLongitude", handleLongitude);
  server.on("/readSpeed", handleSpeed);

  server.begin();
  Serial.println("HTTP server started");
  
  display.init();
  display.flipScreenVertically();

}
unsigned long lastUpdate = 0;
void loop() {
  server.handleClient();
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
