import cv2
import RPi.GPIO as GPIO
import firebase_admin
from firebase_admin import credentials, db, storage
import threading
import time
import os

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(26, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(6, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)


#  khoi tao CAMERA
cap_CH1 = cv2.VideoCapture(0)
cap_CH2 = cv2.VideoCapture(2)
cap_CH3 = cv2.VideoCapture(4)

# Path to your Firebase credentials file
cred = credentials.Certificate('/home/HuynhCongThanh/Documents/HocLapTrinh/thuctapiot-53e65-firebase-adminsdk-amej4-0655c5be5c.json')

# Initialize Firebase app with service account
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://thuctapiot-53e65-default-rtdb.firebaseio.com/',
    'storageBucket': 'thuctapiot-53e65.appspot.com'
})

def check_firebase_connection():
    while True:
        try:
            ref = db.reference('test')
            data = ref.get()
            if data:
                print("Realtime data from Firebase:", data)
                blink_led(13, GPIO.HIGH, 1)  # Blink twice if connected
            else:
                print("No data found in 'test' node.")
                blink_led_err(13, GPIO.LOW, 5)  # Blink once if not connected
        except Exception as e:
            print(f"Failed to read from Firebase: {e}")
            blink_led(GPIO.LOW, 1)  # Blink once if not connected
        time.sleep(0.1)

def blink_led(pin, state, count):
    for _ in range(count):
        GPIO.output(pin, state)
        time.sleep(0.2)
        GPIO.output(pin, not state)
        time.sleep(0.1)

def blink_led_err(pin, state, count):
    for _ in range(count):
        GPIO.output(pin, state)
        time.sleep(0.1)
        GPIO.output(pin, not state)
        time.sleep(0.1)
        
def capture_and_upload_image(channel, cam, pin, label):
    try:
        ret, frame = cam.read()
        if ret:
            image_path = f'/home/HuynhCongThanh/{label}.jpg'
            cv2.imwrite(image_path, frame)
            print(f"Image captured from {label}.")
            
            # Upload to Firebase Storage   
            bucket = storage.bucket()
            blob = bucket.blob(os.path.basename(image_path))
            blob.upload_from_filename(image_path)
            print(f'Image uploaded to Firebase Storage: {blob.name}')
            blink_led(pin, GPIO.HIGH, 1)
        else:
            print(f"Failed to capture image from {label}. Reinitializing camera.")
            blink_led_err(pin, GPIO.LOW, 3)
            reinitialize_camera(cam, label)
    except Exception as e:
        print(f"Error capturing or uploading image from {label}: {e}")
        reinitialize_camera(cam, label)

def reinitialize_camera(cam, label):
    global cap_CH1, cap_CH2, cap_CH3
    cap_CH1.release()
    cap_CH2.release()
    cap_CH3.release()
    cap_CH1 = cv2.VideoCapture(0)
    cap_CH2 = cv2.VideoCapture(2)
    cap_CH3 = cv2.VideoCapture(4)
    
def setup_buttons():
    GPIO.add_event_detect(22, GPIO.FALLING, callback=lambda x: capture_and_upload_image(x, cap_CH1, 26, 'clock1'), bouncetime=300)
    GPIO.add_event_detect(27, GPIO.FALLING, callback=lambda x: capture_and_upload_image(x, cap_CH2, 6, 'clock2'), bouncetime=300)
    GPIO.add_event_detect(17, GPIO.FALLING, callback=lambda x: capture_and_upload_image(x, cap_CH3, 5, 'clock3'), bouncetime=300)
setup_buttons()

# Create a thread for Firebase connection check
firebase_thread = threading.Thread(target=check_firebase_connection)
firebase_thread.start()

# Main loop to keep the program running
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    GPIO.cleanup()
    cap_CH1.release()
    cap_CH2.release()
    cap_CH3.release()