#include <Arduino.h>
#include <U8x8lib.h>

#define SCREEN_WIDTH 16 //OLED display dimensions
#define SCREEN_HEIGHT 8
#define PLAYER_TOKEN '>'

#define NT0 -1

#define NTA0 110
#define NTA0S 117
#define NTB0 123
#define NTC0 130
#define NTC0S 139
#define NTD0 147
#define NTD0S 156
#define NTE0 165
#define NTF0 175
#define NTF0S 185
#define NTG0 196
#define NTG0S 208
#define NTA1 221
#define NTA1S 234
#define NTB1 248
#define NTC1 262
#define NTC1S 278

#define NTD1 294
#define NTD1S 311
#define NTE1 330
#define NTF1 350
#define NTF1S 370
#define NTG1 393
#define NTG1S 415
#define NTA2 441
#define NTA2S 467
#define NTB2 495
#define NTC2 524
#define NTC2S 556

#define NTD2 589
#define NTD2S 623
#define NTE2 661
#define NTF2 700
#define NTF2S 741
#define NTG2 786
#define NTG2S 832
#define NTA3 882
#define NTA3S 934
#define NTB3 990
#define NTB3S 1050
#define NTC3 1110

 
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
 
const static int tune[] =
{
NTB3,NTB3,
NTB3,NTG2S,NTF2S,NTF2,
NTE2,NTF2S,NTE2,
NTA2,
NTF1S,NTB2,NTA2,
NTF1S,
NTE1,NTE1,NTE2,
NTE1,NTE1,NTE2,
NTE1,NTE1,NTE2,
NTB2,
NTC2S,NTF2S,NTE2,
NTG2,
NTF2S,NTB3,NTA3,
NTF2S,NTF2,
NTE2,NTE2,NTE1,
NTE2,NTE1,NTC2S,
NTA2,
NTE2,NTF2S,NTE2,
NTA2,
NTF1S,NTF1S,NTB2,NTA2,
NTF1S,NTF1,
NTE1,NTE1,NTE2,
NT0,NTE1,NTE1,NTE2,
NT0,NTE1,NTD1S,NTE1,NTE1,NTE2,
NTB2,NTC2S,NTD2,
NTC2S,NTC2S,NTF2S,NTE2,
NTB2,
NTF2S,NTF2S,NTB3,NTA3,
NTF2S,NTF2,
NTE2,NTE2,NTE1,
NTE2,NTE1,NTC2S,
NTA2,
NTA2,NTG1S,NTA2,NTB2,NTC2S,NTD2,NTE2,
NTF2S,NTF2,
NTE2,NTF2S,NTC2S,
NTD2,NTE2,
NTC2S,NTE2,NTF2S,NTG2S,
NTA3,NTG2S,NTA3,NTB3,
NTC3,NTE2,NTC3,
NTB3,NTA3,NTG2S,
NTA3

};
 
const static float durt[]=
{
2,2,
1,1,1,1,
2,1,1,
4,
2,1,1,
4,
1.5,1.5,1,
1.5,1.5,1,
1.5,1.5,1,
4,
2,1,1,
4,
2,1,1,
3.5,0.5,
1.5,1,1.5,
1.5,1.5,1,
8,
2,1,1,
4,
1,1,1,1,
3.5,0.5,
1.5,1.5,1,
0.5,1,1.5,1,
0.5,0.5,0.5,0.5,0.5,1.5,
3,0.5,0.5,
1,1,1,1,
4,
1,1,1,1,
3.5,0.5,
1.5,1,1.5,
1.5,1.5,1,
4.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,
2,2,
2,1,1,
2,2,
2.5,0.5,0.5,0.5,
2.5,0.5,0.5,0.5,
2,1,1,
2,1,1,
4
};
 
int length;
int tonepin=5;
int ledp=4;
int x = 0;
int note1 = HIGH;
unsigned long previousMillis1 = 0;
int toneSpeed = 350;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
int soundPin = A2;
int rotaryPin = A0;
int buttonPin = 6;

int buttonState = 0;
int rotaryValue = 0;
int rng = 0;
int carSpeed = 500;
int invulnFrames = 500;
unsigned long currentMillis, carMillis = 0, colisionMillis = 0;

char screenMap[ SCREEN_HEIGHT ][ SCREEN_WIDTH+1 ];

void setup()
{
  Serial.begin(9600);
   u8x8.begin();
  u8x8.setFlipMode(1);
  pinMode(tonepin,OUTPUT);
  pinMode(ledp,OUTPUT);
   pinMode(rotaryPin, INPUT);
  pinMode(soundPin, INPUT);
  pinMode(buttonPin, INPUT);
  length=sizeof(tune)/sizeof(tune[0]);
  noTone(tonepin);
  
}
 
void loop()
{
   currentMillis = millis();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  rotaryValue = analogRead(rotaryPin);
  buttonState = digitalRead(buttonPin);
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    if ( SCREEN_HEIGHT - i - 1 == rotaryValue/128) {
      screenMap[i][0] = PLAYER_TOKEN;
    }
    else {
      screenMap[i][0] = ' ';
    }

    // Slide obstacles
    if (currentMillis - carMillis > carSpeed) {
      for (int j = 1; j < SCREEN_WIDTH; j++){
        if (screenMap[i][j] == '#') {
          // Collision detected
          if ((screenMap[i][j - 1] == PLAYER_TOKEN) && (currentMillis - colisionMillis) >= invulnFrames) {
            carSpeed = 500;
            tone(tonepin, 1200);
            
          }
          
          screenMap[i][j - 1] = '#';
          screenMap[i][j] = ' ';
        }
      }
    }
  }


  // Play Music
   if((note1 == LOW) && ( currentMillis - previousMillis1 >= (toneSpeed/5*durt[x % length]))){ 
      note1 = HIGH;
      previousMillis1 = currentMillis;
      x = 1 + x;
      tone(tonepin,tune[x % length]);
      digitalWrite(ledp, HIGH);
    } else if((note1 == HIGH) && (currentMillis - previousMillis1 >= ((toneSpeed / 5) * 4 *durt[x % length]))){
      note1 = LOW;
      previousMillis1 = currentMillis;
      noTone(tonepin);
      digitalWrite(ledp, LOW);
    }
      // Generate new obstacles
  if (currentMillis - carMillis > carSpeed) {
    rng = analogRead(soundPin) % 8;
    screenMap[rng][SCREEN_WIDTH-1] = '#';
    
    carMillis = currentMillis;
  }

  // Speed up game
  if (buttonState == HIGH) {
    carSpeed = carSpeed * 0.95;
  }
  
  // Print new map
//  Serial.println("=========================");
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    u8x8.setCursor(0, i);
    u8x8.print(screenMap[i]);
//    Serial.println(screenMap[i]);
  }
    
//    tone(tonepin,tune[x]);
//    digitalWrite(ledp, HIGH); 
//    delay(400*durt[x]);
//    digitalWrite(ledp, LOW);
//    delay(100*durt[x]);
//    noTone(tonepin);
//
//  delay(4000);
}
