/*
  PTT Button - ESP32 Serial Version
  ----------------------------------
  Wiring:
    Switch terminal 1  -> GPIO4
    Switch terminal 2  -> GND
  No resistor needed - uses ESP32 internal pull-up.

  Behavior:
    Button pressed  -> sends "START\n" over USB serial (115200 baud)
    Button released -> sends "STOP\n"  over USB serial

  This sketch is used together with ptt_record.py on the laptop.
  The Python script listens on the COM port and starts/stops
  microphone recording accordingly.

  Board: Any standard ESP32 Dev Module (NOT Pro Micro / Leonardo)
  DO NOT use these pins for the button:
    - GPIO44 / GPIO43  (ESP32-S3) -- USB serial RX/TX
    - GPIO1 / TX0      (regular ESP32) -- serial TX
    - GPIO0, 2, 3, 45, 46 -- strapping/boot pins
*/

#define PTT_BUTTON  4    // GPIO connected to one terminal of the button
#define PTT_OUT     5    // Optional: LED / output indicator (can leave unconnected)

const unsigned long DEBOUNCE_MS = 30;

bool lastReading = HIGH;
bool pttState = false;
unsigned long lastChangeTime = 0;

void setup() {
  pinMode(PTT_BUTTON, INPUT_PULLUP);
  pinMode(PTT_OUT, OUTPUT);
  digitalWrite(PTT_OUT, LOW);
  Serial.begin(115200);
}

void loop() {
  bool reading = digitalRead(PTT_BUTTON);

  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  if ((millis() - lastChangeTime) > DEBOUNCE_MS) {
    bool pressed = (reading == LOW);
    if (pressed != pttState) {
      pttState = pressed;
      digitalWrite(PTT_OUT, pttState ? HIGH : LOW);
      Serial.println(pttState ? "START" : "STOP");
    }
  }

  lastReading = reading;
}
