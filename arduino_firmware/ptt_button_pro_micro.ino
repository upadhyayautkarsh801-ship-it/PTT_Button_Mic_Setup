/*
  Push-to-Talk (PTT) button for Arduino Pro Micro
  ------------------------------------------------
  Wiring: one leg of a momentary push button to PIN 2, the other leg to GND.
  No resistor needed - we use the internal pull-up.

  Behavior: while the button is held down, the sketch presses and holds
  PTT_KEY on the PC (acts as a real USB keyboard). Releasing the button
  releases the key. Bind PTT_KEY as your push-to-talk key in Discord,
  TeamSpeak, your game, etc.

  Board: this relies on the Pro Micro's native USB (ATmega32u4), so it
  must be a genuine Pro Micro / Leonardo-class board, not one using a
  separate USB-to-serial chip.
*/

#include <Keyboard.h>

const int BUTTON_PIN = 2;

// The key sent while the button is held. Change this to whatever you've
// bound as push-to-talk in your voice app. Common choices:
//   '`'                -> backtick/grave key
//   KEY_LEFT_CTRL      -> Left Control
//   KEY_F13 ... F24     -> extra function keys (need to #define, see below)
const char PTT_KEY = '`';

const unsigned long DEBOUNCE_MS = 30;

bool lastReading = HIGH;
bool buttonState = HIGH;
bool keyIsDown = false;
unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  // Debounce: only trust the reading after it's been stable for DEBOUNCE_MS
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW && !keyIsDown) {
        // Button pressed (LOW because of INPUT_PULLUP) -> press and hold the key
        Keyboard.press(PTT_KEY);
        keyIsDown = true;
      } else if (buttonState == HIGH && keyIsDown) {
        // Button released -> release the key
        Keyboard.release(PTT_KEY);
        keyIsDown = false;
      }
    }
  }

  lastReading = reading;
}
