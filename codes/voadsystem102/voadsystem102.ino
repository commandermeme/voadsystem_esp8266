#include <ArduinoJson-v5.13.5.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SH1106Wire.h>
#include <TinyGPS++.h>

SH1106Wire display(0x3c, D1, D2);
TinyGPSPlus gps;

const char* ssid = "memz";
const char* password = "Mypreciousone@";
String apikey = "Arr8rRtGsDs4Wd5RmcwJlEBI9VeE7Zc00CfGaKhktcC87JwSfikFInKiVWRVVWKc";
//String latitude = "10.301535";
//String longitude = "123.890853";
unsigned long updateScreen = 0;

void setup() 
{
  Serial.begin(9600);
  
  display.init();
  display.flipScreenVertically();

  headerText();
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Connecting....");
    display.display();
    display.clear();
  }
}

void loop() 
{
  unsigned long now = millis();
  
  while(Serial.available() > 0) {
    if(gps.encode(Serial.read())) {
      if(now - updateScreen > 100) {
        displayGps();
//        apiConnection();
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
  delay(3000);
  display.clear();
}

void displayGps() {
  if(gps.location.isValid()) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  
//    display.drawString(0, 10, "Lat: " + String(gps.location.lat(), 5));
//    display.drawString(0, 20, "Lng: " + String(gps.location.lng(), 5));
//    display.drawString(0, 0, "Aznar Road");
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
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
    String current_speed = String(gps.speed.kmph(), 0);
    
    apiConnection(latitude, longitude, current_speed);
    
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

//String latitude = latDisplay();
//String longitude = longDisplay();

void apiConnection(String latitude, String longitude, String current_speed) {
  if (WiFi.status() == WL_CONNECTED || gps.location.isValid()) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://192.168.1.4/voadsystem/public/api/trackings/latitude=" +latitude+ "&longitude=" +longitude+ "&speed=" +current_speed+ "&system=voad001");
//    http.begin("http://dev.virtualearth.net/rest/v1/locations/" +latitude+ "," +longitude+ "?o=json&key=" +apikey);
//    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());

//      const char* copyright = root["copyright"];
//      const char* street = root["resourceSets"][0]["resources"][0]["address"]["addressLine"]; 
//      const char* adminDistrict2 = root["resourceSets"][0]["resources"][0]["address"]["adminDistrict2"];

        const char* street = root["street"]; 
        const char* speed_limit = root["speed_limit"];
        const char* speed = root["speed"]; 
        const char* violation = root["violation"];
      
//      Serial.print("Copy Right:");
//      Serial.println(copyright);
//      Serial.print("Street:");
//      Serial.println(street);
//      Serial.print("Province:");
//      Serial.println(adminDistrict2);

      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
//      display.drawString(0, 0, String(street)+ "," +String(speed));
      display.drawString(0, 0, String(street)+ "," +String(speed_limit)+ "," +String(speed)+ "," +String(violation));
//      display.display();
//      display.clear();

//      String payload = http.getString();
//      Serial.println(payload);
    }
    else {
      Serial.println("ERROR!");
      display.drawString(0, 0, "Error!");
    }
    http.end(); //Close connection
  }
  delay(1000);
}

String latDisplay() {
  gps.location.lat(), 5;
  delay(1000);
}

String longDisplay() {
  gps.location.lng(), 5;
  delay(1000);
}
