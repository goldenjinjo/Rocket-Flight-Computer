import time
import serial
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



def write_to_serial(ser, message):
    try:
        formatted_message = f"{PREFIX}{message}{SUFFIX}"
        ser.write(formatted_message.encode(ENCODING))
        print_debug(f"Sent: {formatted_message}")
    except serial.SerialException as e:
        print_debug(f"Error writing to serial port: {e}")
    except Exception as e:
        print_debug(f"Unexpected error: {e}")

def read_from_serial(ser):
    return ser.readline().decode(ENCODING).strip()
    


def send_handshake(ser):
    write_to_serial(ser, HANDSHAKE_MESSAGE)
    print(f"Sent handshake message: {HANDSHAKE_MESSAGE.strip()}")
