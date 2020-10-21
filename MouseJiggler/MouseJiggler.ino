#include <Mouse.h>
#include <Adafruit_CircuitPlayground.h>
#include "Pitches.h"
#include "Colors.h"

// Global Constants
#define MOUSE_SPEED 5 // the speed of each mouse movement, lower is faster
#define MOUSE_MOVE_INTERVAL 3000 // how often to move the mouse (in ms)

// Light and Color Global Variables
int lightIndex = 0;
int colors[] = {
  RED, ORANGE, YELLOW, YELLOW_GREEN, GREEN, GREEN_BLUE, BLUE, VIOLET, PURPLE, PINK
};

// Sound Global Variables
int noteIndex = 0;
int noteCount = 7;
int scale[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4
};

enum lightState {
  ON,
  OFF
} lightState = OFF;

enum buttonPress {
  LEFT,
  RIGHT,
  NO
} buttonPress = NO;

void checkForButtonPress() {
  bool leftFirst = CircuitPlayground.leftButton();
  bool rightFirst = CircuitPlayground.rightButton();
  delay(10);
  bool leftSecond = CircuitPlayground.leftButton();
  bool rightSecond = CircuitPlayground.rightButton();

  if (leftFirst && !leftSecond) {
    buttonPress = LEFT;
    return;
  }
  if (rightFirst && !rightSecond) {
    buttonPress = RIGHT;
    return;
  }
  
  buttonPress = NO;
}

void playSound() {
  int duration = 75;
  CircuitPlayground.playTone(scale[random(5)], duration);
  
//  CircuitPlayground.playTone(523, 200);  // 523hz = C4
//  CircuitPlayground.playTone(659, 100);  // 659hz = E5
//  CircuitPlayground.playTone(698, 400);  // 698hz = F5
}

// This turns all lights on or off
void lightSwitch() {
  switch (lightState) {
    case OFF:
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 0x279CD4);
      }
      lightState = ON;
      break;
    case ON:
      CircuitPlayground.clearPixels();
      lightState = OFF;
      break;
  }
}

// This turns on a light each time it's called
void lightIncrement() {
  if (lightIndex > 9) {
    CircuitPlayground.clearPixels();
    lightIndex = 0;
    return;
  }
  // Uncomment this to display a custom array of collors
  //CircuitPlayground.setPixelColor(lightIndex, colors[lightIndex]);
  
  // Uncomment this to display the rainbow
  int color = CircuitPlayground.colorWheel((lightIndex * 256 / 10) & 255);
  CircuitPlayground.setPixelColor(lightIndex, color);
  
  lightIndex++;
}

// This moves the mouse
void wiggleAndJiggle() {
  int distance = random(10, 800);
  int x = random(3) - 1;
  int y = random(3) - 1;

  for (int i = 0; i < distance; i++) {
    Mouse.move(x, y, 0);
    delay(MOUSE_SPEED);
  }
  delay(MOUSE_MOVE_INTERVAL);
}

void setup() {
  Serial.begin(115200);
  Serial.println("This is the setup function!");
  
  CircuitPlayground.begin();
  Mouse.begin();
  randomSeed(analogRead(2));
}

void loop() {
  // If the slide switch is on the left...
  while (CircuitPlayground.slideSwitch()) {
    CircuitPlayground.clearPixels(); // Clear the neopixels
    CircuitPlayground.redLED(HIGH); // Turn on the red led light
    wiggleAndJiggle(); // move the mouse
  }
  CircuitPlayground.redLED(LOW); // Turn off the red led light
  
  // If the slide switch is on the right and a button is pressed...
  checkForButtonPress();
  switch (buttonPress) {
    case NO:
      break;
    case LEFT:
      playSound();
      break;
    case RIGHT:
      lightIncrement();
      break;
  }
}
