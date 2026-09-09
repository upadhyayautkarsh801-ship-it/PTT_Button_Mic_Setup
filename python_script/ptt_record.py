import serial
import sounddevice as sd
import numpy as np
from scipy.io.wavfile import write
import datetime
import os

# ---- CONFIG: change these values ----
COM_PORT = "COM7"
MIC_DEVICE_INDEX = 1
SAVE_FOLDER = r"C:\Users\upadh\Desktop\PTT_Recordings"   # <-- change if you like
# -------------------------------------------

# Create the folder if it doesn't already exist
os.makedirs(SAVE_FOLDER, exist_ok=True)

SAMPLE_RATE = 44100
CHANNELS = 1

ser = serial.Serial(COM_PORT, 115200, timeout=1)
print(f"Listening on {COM_PORT}... waiting for PTT presses.")
print(f"Recordings will be saved to: {SAVE_FOLDER}")

recording_data = []
is_recording = False
stream = None

def audio_callback(indata, frames, time, status):
    if is_recording:
        recording_data.append(indata.copy())

def start_recording():
    global recording_data, is_recording, stream
    recording_data = []
    is_recording = True
    stream = sd.InputStream(
        samplerate=SAMPLE_RATE,
        channels=CHANNELS,
        device=MIC_DEVICE_INDEX,
        callback=audio_callback
    )
    stream.start()
    print("Recording started...")

def stop_recording():
    global is_recording, stream
    is_recording = False
    if stream:
        stream.stop()
        stream.close()

    if recording_data:
        audio = np.concatenate(recording_data, axis=0)
        filename = f"recording_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.wav"
        filepath = os.path.join(SAVE_FOLDER, filename)
        write(filepath, SAMPLE_RATE, audio)
        print(f"Saved: {filepath}")
    else:
        print("No audio captured.")

try:
    while True:
        raw_line = ser.readline()
        if not raw_line:
            continue
        try:
            line = raw_line.decode('utf-8', errors='ignore').strip()
        except Exception:
            continue

        if line == "START":
            start_recording()
        elif line == "STOP":
            stop_recording()
except KeyboardInterrupt:
    print("\nStopped by user.")
    ser.close()