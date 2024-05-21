import serial
from serial.tools import list_ports
import sys
import time

# Function to find the COM port dynamically
def find_com_port():
    ports = list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

# Find the COM port dynamically
com_port = find_com_port()
if com_port is None:
    print("No USB serial port found. Make sure your device is connected.")
    sys.exit()

# Print the connected COM port
print(f"Connected to COM port: {com_port}")

# Open serial port connection
ser = serial.Serial(com_port, 9600, timeout=1)

try:
    # Open file for writing
    with open('output_data.txt', 'w') as f:
        print("Press Ctrl+C to stop the program.")
        while True:
            # Read data from serial port
            data = ser.readline().decode('utf-8').strip()  # Decode bytes to string
            if data:  # Check if data is not empty
                # Write data to file
                f.write(data + '\n')

                # Optionally, print data to console
                print(data)
            time.sleep(0.1)  # Slight delay to allow interrupt

except KeyboardInterrupt:
    print("\nProgram interrupted by user. Exiting...")

finally:
    # Close serial port connection
    ser.close()
    print("Serial port closed.")
