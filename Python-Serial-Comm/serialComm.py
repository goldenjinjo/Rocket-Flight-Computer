# serialComm.py
import serial
from serial.tools import list_ports
from datetime import datetime
import time

# Constants
BAUD_RATE = 115200  # Ensure this matches the flight computer's baud rate
TIMEOUT_SECONDS = 180  # 3 minutes
DEBUG = True  # Set this to True for debugging

# Messages
HANDSHAKE_MESSAGE = "START_TRANSFER\n"
ACK_MESSAGE = "TRANSFER_ACK"
REQUEST_FILE_DOWNLOAD = "REQUEST_FILE_DOWNLOAD\n"

def print_debug(message):
    if DEBUG:
        print(f"{datetime.now()}: {message}")

def find_com_port():
    ports = list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

def send_handshake(ser):
    ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
    print(f"Sent handshake message: {HANDSHAKE_MESSAGE.strip()}")

def send_request_file_download(ser):
    ser.write(REQUEST_FILE_DOWNLOAD.encode('utf-8'))
    print(f"Sent file download request: {REQUEST_FILE_DOWNLOAD.strip()}")

def configure_mode(ser, mode):
    mode_message = f"mode:{mode}\n"
    ser.write(mode_message.encode('utf-8'))
    print_debug(f"Sent mode configuration: {mode_message.strip()}")

def communicate_with_serial(serial_callback):
    # Find the COM port dynamically
    com_port = find_com_port()
    if com_port is None:
        print("No USB serial port found. Make sure your device is connected.")
        return

    print(f"Connected to COM port: {com_port}")

    # Open serial port connection
    ser = serial.Serial(com_port, BAUD_RATE, timeout=0.1)

    try:
        # Clear the serial buffer
        ser.reset_input_buffer()

        # Send handshake message and wait for acknowledgment
        send_handshake(ser)
        start_time = time.time()
        while True:
            if ser.in_waiting > 0:
                response = ser.readline().decode('utf-8').strip()
                print_debug(f"Received response: {response}")
                if response == ACK_MESSAGE:
                    print_debug(f"Received acknowledgment: {ACK_MESSAGE}")
                    break
            if time.time() - start_time > TIMEOUT_SECONDS:
                print_debug("Handshake timed out. Exiting...")
                return

        # Send file download request
        send_request_file_download(ser)

        # Callback for further serial communication
        serial_callback(ser)

    finally:
        # Close serial port connection
        ser.close()
        print_debug("Serial port closed.")
