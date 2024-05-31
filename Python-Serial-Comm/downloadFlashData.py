import serial
from serial.tools import list_ports
import sys
import time
import os
from datetime import datetime
import zlib

# Constants
HANDSHAKE_MESSAGE = "START_TRANSFER\n"
ACK_MESSAGE = "TRANSFER_ACK"
END_OF_TRANSMISSION_MESSAGE = "END_OF_TRANSMISSION"
END_OF_TRANSMISSION_ACK = "END_OF_TRANSMISSION_ACK\n"
FILE_COPY_MESSAGE = "FILE_ALREADY_RECEIVED\n"
ALL_FILES_SENT = "ALL_FILES_SENT"
ALL_FILES_SENT_ACK = "ALL_FILES_SENT_ACK\n"
REQUEST_FILE_DOWNLOAD = "REQUEST_FILE_DOWNLOAD\n"
GO_TO_STANDBY = "mode:0"
TIMEOUT_SECONDS = 180  # 3 minutes
BAUD_RATE = 115200  # Ensure this matches the flight computer's baud rate

# Directory and file naming
OUTPUT_DIRECTORY = "flightData"
LOG_FOLDER = "logFiles"
DATA_FOLDER = "dataFiles"
MISC_FOLDER = "miscFiles"
DEFAULT_FILE_PREFIX = "flight_data_"
LOG_FILE_PREFIX = "log"
DATA_FILE_PREFIX = "data"

DEBUG = True  # Set this to True for debugging

def print_debug(message):
    if DEBUG:
        print(f"{datetime.now()}: {message}")

def ensure_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def sort_file(file_name):
    # Organize file into appropriate folder based on prefix
    file_prefix = file_name.split('_')[0]
    if file_prefix == LOG_FILE_PREFIX:
        file_directory = os.path.join(OUTPUT_DIRECTORY, LOG_FOLDER)
    elif file_prefix == DATA_FILE_PREFIX:
        file_directory = os.path.join(OUTPUT_DIRECTORY, DATA_FOLDER)
    else:
        file_directory = os.path.join(OUTPUT_DIRECTORY, MISC_FOLDER)

    ensure_directory(file_directory)
    
    return os.path.join(file_directory, file_name)

def write_time_to_str():
    # Get the current time as a time.struct_time object
    current_time = time.localtime()
    # Format the current time as a string
    return time.strftime("%Y%m%d_%H%M%S", current_time)

def find_com_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

def send_handshake(ser):
    ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
    print(f"Sent handshake message: {HANDSHAKE_MESSAGE.strip()}")

def communicate_with_serial():
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
        print("No USB serial port found. Make sure your device is connected.")
        sys.exit()

    # Print the connected COM port
    print(f"Connected to COM port: {com_port}")

    # Open serial port connection
    ser = serial.Serial(com_port, BAUD_RATE, timeout=0.1)

    ser.write(REQUEST_FILE_DOWNLOAD.encode('utf-8'))

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
                sys.exit()

        while True:
            file_name_received = False
            file_name = ""
            checksum = 0

            while True:
                if ser.in_waiting > 0:
                    response = ser.readline().decode('utf-8').strip()
                    print_debug(f"Received response: {response}")

                    if response.startswith("FILE_NAME:"):
                        file_name = response[len("FILE_NAME:"):]
                        file_name_received = True
                        print_debug(f"Received file name: {file_name}")
                    elif response.startswith("CHECKSUM:"):
                        checksum = int(response[len("CHECKSUM:"):])
                        print_debug(f"Received checksum: {checksum}")
                        break
                    elif response == ALL_FILES_SENT:
                        # Exit out of code loop after receiving message
                        print_debug("All files have been sent. Sending acknowledgment...")
                        ser.write(ALL_FILES_SENT_ACK.encode('utf-8'))
                        time.sleep(1)
                        ser.write(GO_TO_STANDBY.encode('utf-8'))
                        sys.exit()

            if file_name_received:
                print(file_name)
                file_crc = 0
                output_file_path = sort_file(file_name)

                # Does not write to file if it is found to already exist
                if os.path.exists(output_file_path):
                    print_debug(f"File {file_name} already exists. Skipping download.")
                    ser.write(FILE_COPY_MESSAGE.encode('utf-8'))
                else:
                    # Open file for writing
                    with open(output_file_path, 'w') as f:
                        print_debug(f"Writing data to {output_file_path}")
                        crc = 0
                        while True:
                            # Read data from serial port
                            data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
                            print_debug(f"Received data: {data}")
                            if data == END_OF_TRANSMISSION_MESSAGE.strip():
                                print_debug(f"End of transmission for {file_name} received.")
                                ser.write(END_OF_TRANSMISSION_ACK.encode('utf-8'))
                                break
                            if data:  # Check if data is not empty
                                # Write data to file
                                f.write(data + '\n')
                                crc = zlib.crc32(data.encode('utf-8'), crc)

    except KeyboardInterrupt:
        print_debug("\nProgram interrupted by user. Exiting...")

    finally:
        # Close serial port connection
        ser.close()
        print_debug("Serial port closed.")