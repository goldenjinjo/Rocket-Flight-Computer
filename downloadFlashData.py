# import serial
# from serial.tools import list_ports
# import sys
# import time
# import os
# from datetime import datetime

# # Constants
# HANDSHAKE_MESSAGE = "START_TRANSFER"
# ACK_MESSAGE = "TRANSFER_ACK"
# END_OF_TRANSMISSION_MESSAGE = "END_OF_TRANSMISSION"
# TIMEOUT_SECONDS = 180  # 3 minutes
# BAUD_RATE = 20000

# # Directory and file naming
# OUTPUT_DIRECTORY = "flightData"
# LOG_FOLDER = "logFiles"
# DATA_FOLDER = "dataFiles"
# MISC_FOLDER = "miscFiles"
# DEFAULT_FILE_PREFIX = "flight_data_"
# DEBUG = False  # Set this to True if you want to debug

# # Function to find the COM port dynamically
# def find_com_port():
#     ports = list_ports.comports()
#     for port in ports:
#         if 'USB' in port.description:  # Adjust this condition based on your device's description
#             return port.device
#     return None

# # Function to ensure the directory exists
# def ensure_directory(directory):
#     if not os.path.exists(directory):
#         os.makedirs(directory)

# # Ensure the main output directory exists
# script_directory = os.path.dirname(os.path.abspath(__file__))
# output_directory = os.path.join(script_directory, OUTPUT_DIRECTORY)
# ensure_directory(output_directory)

# # Ensure the misc folder exists
# misc_directory = os.path.join(output_directory, MISC_FOLDER)
# ensure_directory(misc_directory)

# # Generate the default output file name with the current date and time
# current_time = datetime.now().strftime("%Y%m%d_%H%M%S")
# default_file_name = f'{DEFAULT_FILE_PREFIX}{current_time}.txt'
# default_output_file_path = os.path.join(misc_directory, default_file_name)

# # Find the COM port dynamically
# com_port = find_com_port()
# if com_port is None:
#     print("No USB serial port found. Make sure your device is connected.")
#     sys.exit()

# # Print the connected COM port
# print(f"Connected to COM port: {com_port}")

# # Open serial port connection
# ser = serial.Serial(com_port, BAUD_RATE, timeout=1)

# def send_handshake():
#     ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
#     print(f"Sent handshake message: {HANDSHAKE_MESSAGE}")

# try:
#     # Repeatedly send the handshake message until acknowledgment is received or timeout occurs
#     start_time = time.time()
#     while True:
#         send_handshake()
#         time.sleep(1)
#         if ser.in_waiting > 0:
#             response = ser.readline().decode('utf-8').strip()
#             if response == ACK_MESSAGE:
#                 print(f"Received acknowledgment: {ACK_MESSAGE}")
#                 break
#         if time.time() - start_time > TIMEOUT_SECONDS:
#             print("Handshake timed out. Exiting...")
#             sys.exit()

#     # Receive file name from flight computer
#     file_name_received = False
#     file_name = ""
#     while True:
#         if ser.in_waiting > 0:
#             response = ser.readline().decode('utf-8').strip()
#             if response.startswith("FILE_NAME:"):
#                 file_name = response[len("FILE_NAME:"):]
#                 file_name_received = True
#                 break

#     if file_name_received:
#         # Organize file into appropriate folder based on prefix
#         file_prefix = file_name.split('_')[0]
#         if file_prefix == "log":
#             file_directory = os.path.join(output_directory, LOG_FOLDER)
#         elif file_prefix == "data":
#             file_directory = os.path.join(output_directory, DATA_FOLDER)
#         else:
#             file_directory = misc_directory

#         ensure_directory(file_directory)
#         output_file_path = os.path.join(file_directory, file_name)
#     else:
#         output_file_path = default_output_file_path

#     # Open file for writing
#     with open(output_file_path, 'w') as f:
#         print(f"Writing data to {output_file_path}")
#         print("Press Ctrl+C to stop the program.")
#         while True:
#             # Read data from serial port
#             data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
#             if data == END_OF_TRANSMISSION_MESSAGE:
#                 print("End of transmission received. Exiting...")
#                 break
#             if data:  # Check if data is not empty
#                 # Write data to file
#                 f.write(data + '\n')

#                 # Optionally, print data to console
#                 print(data)

# except KeyboardInterrupt:
#     print("\nProgram interrupted by user. Exiting...")

# finally:
#     # Close serial port connection
#     ser.close()
#     print("Serial port closed.")

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
TIMEOUT_SECONDS = 180  # 3 minutes
BAUD_RATE = 20000

# Directory and file naming
OUTPUT_DIRECTORY = "flightData"
LOG_FOLDER = "logFiles"
DATA_FOLDER = "dataFiles"
MISC_FOLDER = "miscFiles"
DEFAULT_FILE_PREFIX = "flight_data_"
DEBUG = True  # Set this to True for debugging

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
    print("No USB serial port found. Make sure your device is connected.")
    sys.exit()

# Print the connected COM port
print(f"Connected to COM port: {com_port}")

# Open serial port connection
ser = serial.Serial(com_port, BAUD_RATE, timeout=1)

def send_handshake():
    ser.write(HANDSHAKE_MESSAGE.encode('utf-8'))
    print(f"Sent handshake message: {HANDSHAKE_MESSAGE}")

try:
    # Send handshake message and wait for acknowledgment
    send_handshake()
    start_time = time.time()
    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            print(f"Received response: {response}")
            if response == ACK_MESSAGE:
                print(f"Received acknowledgment: {ACK_MESSAGE}")
                break
        if time.time() - start_time > TIMEOUT_SECONDS:
            print("Handshake timed out. Exiting...")
            sys.exit()

    while True:
        file_name_received = False
        file_name = ""

        while True:
            if ser.in_waiting > 0:
                response = ser.readline().decode('utf-8').strip()
                print(f"Received response: {response}")
                if response.startswith("FILE_NAME:"):
                    file_name = response[len("FILE_NAME:"):]
                    print(f"Received file name: {file_name}")
                    file_name_received = True
                    break

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
                print(f"Writing data to {output_file_path}")
                while True:
                    # Read data from serial port
                    data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
                    # print(f"Received data: {data}")
                    if data == END_OF_TRANSMISSION_MESSAGE:
                        print(f"End of transmission for {file_name} received.")
                        ser.write(END_OF_TRANSMISSION_ACK.encode('utf-8'))
                        print("Sent Response: ", END_OF_TRANSMISSION_ACK)
                        # time.sleep(0.01)  # Add a short delay to avoid message concatenation
                        break
                    if data:  # Check if data is not empty
                        # Write data to file
                        f.write(data + '\n')

                        # Optionally, print data to console
                        # print(data)
        else:
            break

        # Check if all files have been sent
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            # print(f"Received response: {response}")
            if response == ALL_FILES_SENT:
                print("All files have been sent. Exiting...")
                break

except KeyboardInterrupt:
    print("\nProgram interrupted by user. Exiting...")

finally:
    # Close serial port connection
    ser.close()
    print("Serial port closed.")
