#include <Mouse.h>
#include <Adafruit_CircuitPlayground.h>
#include "Pitches.h"
#include "Colors.h"

// Button Global Variables
bool leftButtonIsDown;
bool rightButtonWasDownPreviousLoop;
bool rightButtonIsDown;
bool rightButtonIsClicked;

// Light, Color, and Sound Global Variables
int lightIteration = 0;
int customColors[] = {RED, ORANGE, YELLOW, YELLOW_GREEN, GREEN, GREEN_BLUE, BLUE, VIOLET, PURPLE, PINK};
int scale[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4};
int frequency = scale[random(5)];
int duration = 15;

void checkButtonStatus() {
  leftButtonIsDown = CircuitPlayground.leftButton();
  rightButtonWasDownPreviousLoop = rightButtonIsDown;
  rightButtonIsDown = CircuitPlayground.rightButton();
  rightButtonIsClicked = !rightButtonIsDown && rightButtonWasDownPreviousLoop;
}

// This turns on a light each time it's called
void lightIncrement() {
  if (lightIteration > 19) { lightIteration = 0; }
  int lightNum;
  int color;
  
  if (lightIteration > 9) {
    lightNum = lightIteration - 10;
    color = OFF;
  } else {
    lightNum = lightIteration;
    // color = customColors[lightNum]; // display a custom array of colors
    color = CircuitPlayground.colorWheel((lightNum * 256 / 10) & 255); // display the rainbow
  }
  CircuitPlayground.setPixelColor(lightNum, color);
  lightIteration++;
}

// This moves the mouse
void wiggleAndJiggle() {
  int distance = random(10, 50);
  int x = random(3) - 1;
  int y = random(3) - 1;
  int mouseSpeed = 5; // the speed of each mouse movement, lower is faster
  int mouseMoveInterval = 3000; // how often to move the mouse (in ms)

  for (int i = 0; i < distance; i++) {
    Mouse.move(x, y, 0);
    delay(mouseSpeed);
  }
  delay(mouseMoveInterval);
}

void setup() {
  Serial.begin(115200);  
  CircuitPlayground.begin();
  Mouse.begin();
  randomSeed(analogRead(2));
}

void loop() {
  // If the slide switch is on the left...
  while (CircuitPlayground.slideSwitch()) {
    CircuitPlayground.clearPixels();
    CircuitPlayground.redLED(HIGH); // Turn on the red led light
    wiggleAndJiggle(); // move the mouse
  }
  CircuitPlayground.redLED(LOW); // Turn off the red led light

  // If the slide switch is on the right...
  checkButtonStatus();
  if (leftButtonIsDown) { 
    CircuitPlayground.playTone(frequency, duration);
  } else { 
    frequency = scale[random(5)]; // update the frequency played each press
  }
  if (rightButtonIsClicked) {
    lightIncrement();
  }
}
