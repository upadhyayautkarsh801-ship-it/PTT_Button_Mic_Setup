# PTT Button + USB Microphone — Setup Package

## What's in this folder

`
PTT_Button_Mic_Setup/
├── arduino_firmware/
│   ├── ptt_esp32_serial.ino      <- Use this with ESP32 + ptt_record.py  [MAIN SETUP]
│   └── ptt_button_pro_micro.ino  <- Alternative: Pro Micro keyboard emulation (no Python needed)
├── python_script/
│   └── ptt_record.py             <- Python recording script (for ESP32 serial setup)
├── sample_recordings/
│   └── recording_20260905_231121.wav   <- Example recording for reference
└── README.md                     <- This file
`

---

## Which sketch do I use?

| Sketch | Board | How it works |
|---|---|---|
| ptt_esp32_serial.ino | ESP32 Dev Module | Sends START/STOP over serial -> Python records mic |
| ptt_button_pro_micro.ino | Arduino Pro Micro / Leonardo | Acts as USB keyboard, presses a key while button held |

For the setup described in this guide: use ptt_esp32_serial.ino + ptt_record.py.

---

## 1. Hardware

- ESP32 Dev Board
- R13-507 momentary push button (no-lock, 2 terminals used)
- USB microphone (plugs into laptop directly, NOT into ESP32)
- USB cable (ESP32 to laptop)
- Windows laptop

### Wiring

    Switch terminal 1  ->  GPIO4
    Switch terminal 2  ->  GND

No resistor needed — the code uses the ESP32's internal pull-up resistor.

Avoid these pins for buttons:
  - GPIO44 / GPIO43 (ESP32-S3) — USB serial RX/TX
  - GPIO1 / TX0 (regular ESP32) — serial TX
  - GPIO0, 2, 3, 45, 46 — strapping/boot pins

---

## 2. Arduino IDE Setup

1. Install Arduino IDE: https://arduino.cc/en/software
2. File -> Preferences -> Additional Board Manager URLs, add:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Tools -> Board -> Boards Manager -> search "esp32" -> install (by Espressif Systems)
4. Plug in ESP32 via USB
5. Tools -> Board -> select your exact board (e.g. "ESP32 Dev Module")
6. Tools -> Port -> select the COM port that appeared (e.g. COM7)
7. Open ptt_esp32_serial.ino, click Upload (arrow icon)
8. After upload, CLOSE the Serial Monitor before running the Python script

---

## 3. Python Setup (Windows)

1. Install Python from https://python.org/downloads
   IMPORTANT: check "Add Python to PATH" during install
2. Verify: open Command Prompt, run: python --version
3. Install required packages:
   pip install pyserial sounddevice scipy numpy

---

## 4. Configure ptt_record.py

Open python_script/ptt_record.py and update these three values at the top:

    COM_PORT = "COM7"           <- your ESP32's COM port (check Device Manager)
    MIC_DEVICE_INDEX = 1        <- your USB mic's device index (see below)
    SAVE_FOLDER = r"C:\Users\YourUsername\Desktop\PTT_Recordings"

Find your mic's device index:
    python -c "import sounddevice as sd; print(sd.query_devices())"
Look for your USB mic's name, note the number on the left.

Find your ESP32's COM port:
    Device Manager -> Ports (COM & LPT) -> note the COMx number.

---

## 5. Run and Test

1. Make sure Arduino Serial Monitor is CLOSED
2. Open Command Prompt and run:
       python ptt_record.py
3. You should see: "Listening on COMx... waiting for PTT presses."
4. Press the button  -> "Recording started..."
5. Release the button -> "Saved: <path>\recording_....wav"
6. Open the .wav file in File Explorer to verify audio was captured

---

## 6. Troubleshooting

  "PermissionError" on COM port
    -> Close Arduino Serial Monitor; unplug and replug ESP32

  "No audio captured"
    -> Wrong MIC_DEVICE_INDEX. Re-run: python -c "import sounddevice as sd; print(sd.query_devices())"

  Script won't accept typed input while running
    -> Normal. Press Ctrl+C to stop.

  Can't find the .wav file
    -> Check the path printed at startup; sort File Explorer by Date Modified

---

## Moving to Another PC

1. Copy this entire PTT_Button_Mic_Setup/ folder to the new PC
2. Re-do steps 2 (Arduino IDE) and 3 (Python packages) on the new PC
3. Update COM_PORT and MIC_DEVICE_INDEX in ptt_record.py (they may differ)
4. The ESP32 firmware is already on the board — no need to re-upload unless you got a new board
