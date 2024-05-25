import serial
from serial.tools import list_ports
import sys
import time
import os
from datetime import datetime

# Constants
HANDSHAKE_MESSAGE = "START_TRANSFER"
ACK_MESSAGE = "TRANSFER_ACK"
END_OF_TRANSMISSION_MESSAGE = "END_OF_TRANSMISSION"
END_OF_TRANSMISSION_ACK = "END_OF_TRANSMISSION_ACK"
ALL_FILES_SENT = "ALL_FILES_SENT"
ALL_FILES_SENT_ACK = "ALL_FILES_SENT_ACK"
TIMEOUT_SECONDS = 180  # 3 minutes
BAUD_RATE = 2000000  # 2 Mbps

# Directory and file naming
OUTPUT_DIRECTORY = "flightData"
LOG_FOLDER = "logFiles"
DATA_FOLDER = "dataFiles"
MISC_FOLDER = "miscFiles"
DEFAULT_FILE_PREFIX = "flight_data_"
DEBUG = True  # Set this to True for debugging

def print_debug(message):
    if DEBUG:
        print(message)

# Function to find the COM port dynamically
def find_com_port():
    ports = list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

# Function to ensure the directory exists
def ensure_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

# Ensure the main output directory exists
script_directory = os.path.dirname(os.path.abspath(__file__))
output_directory = os.path.join(script_directory, OUTPUT_DIRECTORY)
ensure_directory(output_directory)

# Ensure the misc folder exists
misc_directory = os.path.join(output_directory, MISC_FOLDER)
ensure_directory(misc_directory)

# Find the COM port dynamically
com_port = find_com_port()
if com_port is None:
    print_debug("No USB serial port found. Make sure your device is connected.")
    sys.exit()

# Print the connected COM port
print_debug(f"Connected to COM port: {com_port}")

# Open serial port connection
ser = serial.Serial(com_port, BAUD_RATE, timeout=1)

def send_handshake():
    ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
    print_debug(f"Sent handshake message: {HANDSHAKE_MESSAGE}")

try:
    # Send handshake message and wait for acknowledgment
    send_handshake()
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
            sys.exit()

    while True:
        file_name_received = False
        file_name = ""

        while True:
            
            if ser.in_waiting > 0:
                response = ser.readline().decode('utf-8').strip()
                print_debug(f"Received response: {response}")
                if response.startswith("FILE_NAME:"):
                    file_name = response[len("FILE_NAME:"):]
                    print_debug(f"Received file name: {file_name}")
                    file_name_received = True
                    break

                # exit out of code loop after recieving message
                if response == ALL_FILES_SENT:
                    print_debug("All files have been sent. Sending acknowledgment...")
                    ser.write(ALL_FILES_SENT_ACK.encode('utf-8'))
                    sys.exit();


        if file_name_received:
            # Organize file into appropriate folder based on prefix
            file_prefix = file_name.split('_')[0]
            if file_prefix == "log":
                file_directory = os.path.join(output_directory, LOG_FOLDER)
            elif file_prefix == "data":
                file_directory = os.path.join(output_directory, DATA_FOLDER)
            else:
                file_directory = misc_directory

            ensure_directory(file_directory)
            output_file_path = os.path.join(file_directory, file_name)

            # Open file for writing
            with open(output_file_path, 'w') as f:
                print_debug(f"Writing data to {output_file_path}")
                while True:
                    # Read data from serial port
                    data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
                    print_debug(f"Received data: {data}")
                    if data == END_OF_TRANSMISSION_MESSAGE:
                        print_debug(f"End of transmission for {file_name} received.")
                        break
                    if data:  # Check if data is not empty
                        # Write data to file
                        f.write(data + '\n')

                        # Optionally, print data to console
                        print_debug(data)
        else:
            break
           

except KeyboardInterrupt:
    print_debug("\nProgram interrupted by user. Exiting...")

finally:
    # Close serial port connection
    ser.close()
    print_debug("Serial port closed.")
