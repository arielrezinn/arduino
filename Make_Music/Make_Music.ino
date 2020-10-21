#include <Adafruit_CircuitPlayground.h>
#define BUTTONA 4
#define BUTTONB 5


void playMusic() {
  CircuitPlayground.playTone(523, 200);  // 523hz = C4
  CircuitPlayground.playTone(659, 100);  // 659hz = E5
  CircuitPlayground.playTone(698, 400);  // 698hz = F5
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Music!");
  pinMode(BUTTONA, INPUT_PULLDOWN);
  pinMode(BUTTONB, INPUT_PULLDOWN);
  CircuitPlayground.begin(); 
  playMusic();
}

void loop() {
  // put your main code here, to run repeatedly:
}
