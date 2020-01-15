#include "SH1106Wire.h"

SH1106Wire display(0x3c, D1, D2);

int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void millisDisplay() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, String(millis()));
}

unsigned long seconds;

void loop() {
  display.clear();
  millisDisplay();
  drawProgressBarDemo();
  seconds = millis();
  Serial.println(seconds);
//  display.setFont(ArialMT_Plain_10);
//  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  counter++;
  delay(1000);
}
