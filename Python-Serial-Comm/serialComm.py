# serialComm.py
import serial
from serial.tools import list_ports
from datetime import datetime
import time
import sys
import threading
from constants import *
from config import *
from helperFunc import *
from downloadFlashData import download_flash_data


stop_threads = False

def find_com_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

def read_from_serial(ser):
    global stop_threads
    while not stop_threads:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {line}")
            time.sleep(0.1)  # Short delay to prevent high CPU usage

def write_to_serial(ser):
    global stop_threads
    while not stop_threads:
        try:
            message = input("Enter message to send: ")
            ser.write((message + '\n').encode('utf-8'))
        except EOFError:
            stop_threads = True


def continuous_serial(ser):
    global stop_threads
    try:
        # Create threads for reading and writing
        read_thread = threading.Thread(target=read_from_serial, args=(ser,))
        write_thread = threading.Thread(target=write_to_serial, args=(ser,))

        # Start the threads
        read_thread.start()
        write_thread.start()

        # Keep the main thread running while handling KeyboardInterrupt
        while not stop_threads:
            time.sleep(0.1)

        # Join the threads to the main thread
        read_thread.join()
        write_thread.join()

    except KeyboardInterrupt:
        print("Stopping the serial interface...")
        # artifact from broken method
        ser.write("stop/n".encode('utf-8'))
        stop_threads = True
    finally:
        # Close the serial port
        if ser.is_open:
            ser.close()
        print("Serial port closed.")


def communicate_with_serial(string):
    
    # Find the COM port dynamically
    com_port = find_com_port()
    if com_port is None:
        print("No USB serial port found. Make sure your device is connected.")
        sys.exit()

    # Print the connected COM port
    print(f"Connected to COM port: {com_port}")

    # Open serial port connection
    ser = serial.Serial(com_port, BAUD_RATE, timeout=0.1)

    select_serial_action(string, ser)



def select_serial_action(string, ser):
    global stop_threads

    if string in MODE_MSG_ARRAY:
        ser.write(string.encode('utf-8'))
    else:
        print(string)
        print(': Not a recognised action.')
        return
    
    if string == GO_TO_READ:
        user_input = input("Would you like to request a file download? (yes/no): ").strip().lower()
        if user_input == 'yes':
            ser.write(REQUEST_FILE_DOWNLOAD.encode('utf-8'))
            download_flash_data(ser)
        else:
            print("Files will not be downloaded. Returning to standby.")
    
    if string == GO_TO_STANDBY:
        print("Going to standby")

    if string == GO_TO_PURGE:
        print("Purging all data from flash. RIP.")

    if string == GO_TO_LOGGING:
        print("Begin Logging Sequence")
    
    if string == GO_TO_FINS:
        user_input = input("Would you like to activate manual fin control? (yes/no): ").strip().lower()
        if user_input == 'yes':
            ser.write("start\n".encode('utf-8'))
            continuous_serial(ser)
        else:
            print("Fins will not move. Returning to standby.")
            ser.write(GO_TO_STANDBY.encode('utf-8'))
        
        
       
    

