#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);
uint8_t bmap[1024];
int idx = 0;
int inputUpdate = 0;

void setup() {
  Serial.begin(9600);

  pinMode(A0, OUTPUT);

  u8g2.begin();
  u8g2.clear();
  u8g2.setColorIndex(1);
  u8g2.setFlipMode(1);

  u8g2.firstPage();

  inputUpdate = millis();
}

void loop() {
  if (millis() - inputUpdate > 500) {
    inputUpdate = millis();
    Serial.println(analogRead(A0));
  }
  
  if (Serial.available() > 0) {
    bmap[idx] = Serial.read();
    idx += 1;
  }

  if (idx == 1024) {
    idx = 0;
    u8g2.firstPage();
    do {
      u8g2.drawXBM(0, 0, 128, 64, bmap);
    } while (u8g2.nextPage());
  }
}
