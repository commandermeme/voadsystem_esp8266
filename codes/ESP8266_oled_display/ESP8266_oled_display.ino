#include <SH1106Wire.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
SH1106Wire display(0x3c, D1, D2);

void setup() {
  Serial.begin(9600);
  
  display.init();
  display.flipScreenVertically();

  headerText();
}

void loop() {
  while(Serial.available() > 0) {
    if(gps.encode(Serial.read())) {
      if(gps.speed.isUpdated()) {
        displayGps();
      }
    }
  }
}

void headerText() {
  // Header text
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "CPE 516");
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 24, "Mon-Wed-Fri");
  display.drawString(64, 40, "5:30-7:30");
  display.display();
  delay(5000);
  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "Baclayon");
  display.drawString(64, 20, "Manloloyo");
  display.drawString(64, 40, "Lagramada");
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
    display.drawString(0, 0, "Lat: " + String(gps.location.lat(), 6));
    display.drawString(0, 10, "Lng: " + String(gps.location.lng(), 6));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 0, "Sat: " + String(gps.satellites.value()));
  
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 20, String(gps.speed.kmph()));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(128, 25, "km/h");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 54, String(gps.time.hour())+ ":" + String(gps.time.minute())+ ":" + String(gps.time.second()));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 54, String(gps.date.month())+ ":" + String(gps.date.day())+ ":" + String(gps.date.year()));
    
//    display.drawString(0, 40, "Time: " + String(gps.time.hour())+ ":" + String(gps.time.minute())+ ":" + String(gps.time.second()));
//    display.drawString(0, 50, "Date: " + String(gps.date.month())+ ":" + String(gps.date.day())+ ":" + String(gps.date.year()));
    
    display.display();
    display.clear();
  }
  else {
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, "NO");
    display.drawString(64, 20, "SATELLITES");
    display.drawString(64, 40, "CONNECTION");
    display.display();
    display.clear(); 
  }
}
