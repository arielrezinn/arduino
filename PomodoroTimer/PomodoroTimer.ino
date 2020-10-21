#include <Adafruit_CircuitPlayground.h>
#define BUTTONA 4
#define BUTTONB 5
#define POMODORO_COLOR 0xB82000
#define BREAK_COLOR 0x279CD4
#define ONE_SECOND 1000
#define LIGHT_OFF 0x000000

#define COUNTDOWN_RATE_LONG_BREAK 180000 // 30 min breaks
#define COUNTDOWN_RATE_SHORT_BREAK 30000 // 5 min breaks
#define COUNTDOWN_RATE_POMODORO 150000 // 25 min pomodoro

//#define COUNTDOWN_RATE_LONG_BREAK 6000 // 1 min breaks
//#define COUNTDOWN_RATE_SHORT_BREAK 1000 // 10 sec breaks
//#define COUNTDOWN_RATE_POMODORO 5000 // 50 sec pomodoro

// Global Variables
long timeOfLastLightIndexDecrement = 0L;
long timeOfLastLightBlink = 0L;
int pomodoroCounter = 0;
int lightIndex = 9;
int lastStateA = 0;
int lastStateB = 0;

enum BlinkingLightState {
  ON,
  OFF
} blinkingLightState = ON;

enum SessionState {
  POMODORO,
  BREAK,
  STOPPED
} sessionState = STOPPED;

void startPomodoro() {
  Serial.println("first line of startPomodoro");
  Serial.print("pomodoro # ");
  Serial.println(pomodoroCounter);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, POMODORO_COLOR);
  }
  lightIndex = 9;
  sessionState = POMODORO;
}

void startBreak() {
  Serial.println("first line of startBreak");
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, BREAK_COLOR);
  }
  lightIndex = 9;
  sessionState = BREAK;
}

void stopSession() {
  Serial.println("first line of stopSession");
  CircuitPlayground.clearPixels();
  lightIndex = 9;
  pomodoroCounter = 0;
  sessionState = STOPPED;
  timeOfLastLightIndexDecrement = 0L;
}

void playSound() {
  CircuitPlayground.playTone(523, 200);  // 523hz = C4
  CircuitPlayground.playTone(659, 100);  // 659hz = E5
  CircuitPlayground.playTone(698, 400);  // 698hz = F5
}

void countdown(int countdownRate) {
  //Serial.println("first line of countdown");
  long currentTime = millis();
  if ((currentTime - timeOfLastLightIndexDecrement) > countdownRate) {
    timeOfLastLightIndexDecrement = currentTime;
    Serial.print("light index: ");
    Serial.println(lightIndex);
    CircuitPlayground.setPixelColor(lightIndex, LIGHT_OFF);
    lightIndex--;
    if (lightIndex < 0) {
      // play sound and flash lights until a button is pressed 
      // then continue to switch statement
      playSound();
      delay(100);
      playSound();
      delay(100);
      playSound();
      
      switch(sessionState) {
        case POMODORO:
          pomodoroCounter++;
          startBreak();
          break;
        case BREAK:
          if (pomodoroCounter == 4) {
            stopSession(); 
          } else {
            startPomodoro();
          }
          break;
        case STOPPED:
          break;
      }
    }
  } else {
    if ((currentTime - timeOfLastLightBlink) > ONE_SECOND) {
      timeOfLastLightBlink = currentTime;
      if (blinkingLightState == ON) {
        CircuitPlayground.setPixelColor(lightIndex, LIGHT_OFF);
        blinkingLightState = OFF;
      } else {
        blinkingLightState = ON;
        switch(sessionState) {
          case POMODORO:
            CircuitPlayground.setPixelColor(lightIndex, POMODORO_COLOR);
            break;
          case BREAK:
            CircuitPlayground.setPixelColor(lightIndex, BREAK_COLOR);
            break;
          case STOPPED:
            break;
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Pomodoro Timer!");
  pinMode(BUTTONA, INPUT_PULLDOWN);
  pinMode(BUTTONB, INPUT_PULLDOWN);
  CircuitPlayground.begin(); 
  CircuitPlayground.clearPixels();
}

void loop() {
  //Serial.println("first line of loop()");
  int stateA = digitalRead(BUTTONA);
  int stateB = digitalRead(BUTTONB);

  delay(100);

  Serial.print("last stateA: ");
  Serial.println(lastStateA);
  Serial.print("last stateB: ");
  Serial.println(lastStateB);
  Serial.print("stateA: ");
  Serial.println(stateA);
  Serial.print("stateB: ");
  Serial.println(stateB);
  
  // if button A has a change of state and is not currently pressed...
  if ((stateA != lastStateA) && stateA == 0) {
    switch(sessionState) {
      case POMODORO:
        Serial.println("loop() first switch: pomodoro");
        break;
      case BREAK:
        Serial.println("loop() first switch: break");
        break;
      case STOPPED:
        Serial.println("loop() first switch: stopped");
        startPomodoro();
        break;
    }
    Serial.println("button A pressed");
    delay(100);
  }

  // if button B has a change of state and is not currently pressed...
  if ((stateB != lastStateB) && stateB == 0) {
    stopSession();
    Serial.println("button B press");
    delay(100);
  }

  // update the previous button state
  lastStateA = stateA;
  lastStateB = stateB;

  switch(sessionState) {
    case POMODORO:
      //Serial.println("loop() second switch: pomodoro");
      countdown(COUNTDOWN_RATE_POMODORO);
      break;
    case BREAK:
      Serial.println("loop() second switch: break");
      if (pomodoroCounter < 4) {
        countdown(COUNTDOWN_RATE_SHORT_BREAK);
      } else {
        countdown(COUNTDOWN_RATE_LONG_BREAK);
      }
      break;
    case STOPPED:
      Serial.println("loop() second switch: stopped");
      break;
    }
}
