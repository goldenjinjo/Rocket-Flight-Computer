import serial
from serial.tools import list_ports
import sys
import time
import os
from datetime import datetime

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

# Directory where the output file will be stored
script_directory = os.path.dirname(os.path.abspath(__file__))
output_directory = os.path.join(script_directory, "flightData")
ensure_directory(output_directory)

# Generate the output file name with the current date and time
current_time = datetime.now().strftime("%Y%m%d_%H%M%S")
output_file_path = os.path.join(output_directory, f'flight_data_{current_time}.txt')

# Find the COM port dynamically
com_port = find_com_port()
if com_port is None:
    print("No USB serial port found. Make sure your device is connected.")
    sys.exit()

# Print the connected COM port
print(f"Connected to COM port: {com_port}")

# Open serial port connection
ser = serial.Serial(com_port, 20000, timeout=1)

# Handshake messages
handshake_message = "START_TRANSFER"
ack_message = "TRANSFER_ACK"
end_of_transmission_message = "END_OF_TRANSMISSION"
timeout_seconds = 180 # 3 minutes

def send_handshake():
    ser.write(handshake_message.encode('utf-8'))
    print(f"Sent handshake message: {handshake_message}")

try:
    # Repeatedly send the handshake message until acknowledgment is received or timeout occurs
    start_time = time.time()
    while True:
        send_handshake()
        time.sleep(1)
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            if response == ack_message:
                print(f"Received acknowledgment: {ack_message}")
                break
        if time.time() - start_time > timeout_seconds:
            print("Handshake timed out. Exiting...")
            sys.exit()

    # Receive file name from flight computer
    file_name_received = False
    file_name = ""
    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            if response.startswith("FILE_NAME:"):
                file_name = response[len("FILE_NAME:"):]
                file_name_received = True
                break

    if file_name_received:
        # Organize file into appropriate folder based on prefix
        file_prefix = file_name.split('_')[0]
        if file_prefix == "log":
            file_directory = os.path.join(output_directory, "logFiles")
        elif file_prefix == "data":
            file_directory = os.path.join(output_directory, "dataFiles")
        else:
            file_directory = os.path.join(output_directory, "miscFiles")

        ensure_directory(file_directory)
        output_file_path = os.path.join(file_directory, file_name)

        # Open file for writing
        with open(output_file_path, 'w') as f:
            print(f"Writing data to {output_file_path}")
            print("Press Ctrl+C to stop the program.")
            while True:
                # Read data from serial port
                data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
                if data == end_of_transmission_message:
                    print("End of transmission received. Exiting...")
                    break
                if data:  # Check if data is not empty
                    # Write data to file
                    f.write(data + '\n')

                    # Optionally, print data to console
                    print(data)

except KeyboardInterrupt:
    print("\nProgram interrupted by user. Exiting...")

finally:
    # Close serial port connection
    ser.close()
    print("Serial port closed.")
