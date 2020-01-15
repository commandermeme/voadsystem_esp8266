#include <TinyGPS++.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

TinyGPSPlus gps;

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(0,0);
  display.println("CPE 516");
  display.setTextSize(1);
  display.setCursor(0,30);
  display.println("Baclayon, J.J");
  display.setCursor(0,40);
  display.println("Manloloyo, L.J");
  display.setCursor(0,50);
  display.println("Lagramada, J.");
  display.display();
  delay(5000);
  display.clearDisplay();
}

void loop() {
  while(Serial.available() > 0) {
    if(gps.encode(Serial.read())){
      displayInfo();
    }
  }
}

void displayInfo() {
  Serial.print("Location: ");
  if(gps.location.isValid()) {
    Serial.print(gps.location.lat(), 8);
    Serial.print(", ");
    Serial.print(gps.location.lng(), 8);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Sat:");
    display.setCursor(24,0);
    display.print(gps.satellites.value());
    display.setCursor(0,8);
    display.print("Lat:");
    display.setCursor(24,8);
    display.print(gps.location.lat());
    display.setCursor(0,16);
    display.print("Lng:");
    display.setCursor(24,16);
    display.print(gps.location.lng());
    display.setCursor(0,25);
    display.setTextSize(3);
    display.print(gps.speed.kmph());
    display.setTextSize(1);
    display.setCursor(100,31);
    display.print("km/h");
    display.setCursor(0,48);
    display.print("Time:");
    display.setCursor(30,48);
    display.print(gps.time.hour());
    display.setCursor(42,48);
    display.print(":");
    display.setCursor(48,48);
    display.print(gps.time.minute());
    display.setCursor(60,48);
    display.print(":");
    display.setCursor(66,48);
    display.print(gps.time.second());
    display.setCursor(0,56);
    display.print("Date:");
    display.setCursor(30,56);
    display.print(gps.date.month());
    display.setCursor(44,56);
    display.print("-");
    display.setCursor(52,56);
    display.print(gps.date.day());
    display.setCursor(66,56);
    display.print("-");
    display.setCursor(74,56);
    display.print(gps.date.year());
    display.display();
    display.clearDisplay();
  }
  else {
    Serial.print("INVALID");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Sat:");
    display.setCursor(24,0);
    display.print("NO CONNECTION!");
    display.setCursor(0,8);
    display.print("Lat:");
    display.setCursor(24,8);
    display.print("-----");
    display.setCursor(0,16);
    display.print("Lng:");
    display.setCursor(24,16);
    display.print("-----");
    display.setCursor(0,25);
    display.setTextSize(3);
    display.print("00.00");
    display.setTextSize(1);
    display.setCursor(100,31);
    display.print("km/h");
    display.setCursor(0,48);
    display.print("Time:");
    display.setCursor(30,48);
    display.print("00:00:00");
    display.setCursor(0,56);
    display.print("Date:");
    display.setCursor(30,56);
    display.print("00-00-00");
    display.display();
    display.clearDisplay();
  }
  Serial.println();
}
