import time
from datetime import datetime
from config.config import DEBUG
from constants.constants import *


def print_debug(message):
    if DEBUG:
        print(f"{datetime.now()}: {message}")

def write_time_to_str():
    # Get the current time as a time.struct_time object
    current_time = time.localtime()
    # Format the current time as a string
    return time.strftime("%Y%m%d_%H%M%S", current_time)

def send_handshake(ser):
    ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
    print(f"Sent handshake message: {HANDSHAKE_MESSAGE.strip()}")