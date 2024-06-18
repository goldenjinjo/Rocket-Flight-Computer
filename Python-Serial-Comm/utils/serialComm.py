# serialComm.py
import serial
from serial.tools import list_ports
from datetime import datetime
import time
import sys
import threading
from constants.constants import *
from config.config import *
from utils.helperFunc import *
from utils.downloadFlashData import download_flash_data

stop_threads = False

def find_com_port():
    ports = list_ports.comports()
    for port in ports:
        if 'USB' in port.description:  # Adjust this condition based on your device's description
            return port.device
    return None

def read_from_serial_continuous(ser):
    global stop_threads
    while not stop_threads:
        try:
            if ser.in_waiting > 0:
                line = read_from_serial(ser)
                print(f"Received: {line}")
                time.sleep(0.1)  # Short delay to prevent high CPU usage
        except serial.SerialException as e:
            print_debug(f"Error reading from serial port: {e}")
            stop_threads = True
        except Exception as e:
            print_debug(f"Unexpected error: {e}")
            stop_threads = True
        finally:
            if stop_threads:
                print_debug("Stopping read thread due to an error.")
                break

def user_input_to_serial(ser):
    global stop_threads
    while not stop_threads:
        try:
            message = input("Enter message to send: ")
            write_to_serial(ser, message)
        except EOFError:
            stop_threads = True
            print_debug("EOFError: Message failed to send")
        except serial.SerialException as e:
            print_debug(f"Error writing to serial port: {e}")
            stop_threads = True
        except Exception as e:
            print_debug(f"Unexpected error: {e}")
            stop_threads = True
        finally:
            if stop_threads:
                print_debug("Stopping write thread due to an error.")
                break

def continuous_serial(ser):
    global stop_threads
    try:
        read_thread = threading.Thread(target=read_from_serial_continuous, args=(ser,))
        write_thread = threading.Thread(target=user_input_to_serial, args=(ser,))

        read_thread.start()
        write_thread.start()

        while not stop_threads:
            time.sleep(0.1)

        read_thread.join()
        write_thread.join()

    except KeyboardInterrupt:
        print("Stopping the serial interface...")
        stop_threads = True
    except Exception as e:
        print_debug(f"Unexpected error: {e}")
        stop_threads = True
    finally:
        try:
            write_to_serial(ser,"end")
            time.sleep(0.5)
            write_to_serial(ser, GO_TO_STANDBY)
        except Exception as e:
            print_debug(f"Error during shutdown: {e}")
        if ser.is_open:
            ser.close()
        print("Serial port closed.")

def communicate_with_serial(string):
    com_port = find_com_port()
    if com_port is None:
        print("No USB serial port found. Make sure your device is connected.")
        sys.exit()

    print(f"Connected to COM port: {com_port}")

    try:
        ser = serial.Serial(com_port, BAUD_RATE, timeout=0.1)
    except serial.SerialException as e:
        print_debug(f"Error opening serial port: {e}")
        sys.exit()

    select_serial_action(string, ser)

def select_serial_action(string, ser):
    global stop_threads

    if string in MODE_MSG_ARRAY:
        try:
            write_to_serial(ser, string)
        except serial.SerialException as e:
            print_debug(f"Error writing to serial port: {e}")
            return
        except Exception as e:
            print_debug(f"Unexpected error: {e}")
            return
    else:
        print(f"{string}: Not a recognized action.")
        return
    
    if string == GO_TO_READ:
        user_input = input("Would you like to request a file download? (yes/no): ").strip().lower()
        if user_input == 'yes':
            try:
                write_to_serial(ser, REQUEST_FILE_DOWNLOAD)
                download_flash_data(ser)
            except serial.SerialException as e:
                print_debug(f"Error during file download: {e}")
            except Exception as e:
                print_debug(f"Unexpected error: {e}")
        else:
            print("Files will not be downloaded. Returning to standby.")
            time.sleep(3)
            try:
                write_to_serial(ser, CANCEL_MSG_REQUEST)
                write_to_serial(ser, GO_TO_STANDBY)
            except Exception as e:
                print_debug(f"Error writing to serial port: {e}")
    
    if string == GO_TO_STANDBY:
        print("Going to standby")

    if string == GO_TO_PURGE:
        print("Purging all data from flash. RIP.")

    if string == GO_TO_LOGGING:
        print("Begin Logging Sequence")
    
    if string == GO_TO_FINS:
        stop_threads = False
        try:
            write_to_serial(ser, "start")
            continuous_serial(ser)
        except serial.SerialException as e:
            print_debug(f"Error during fins control: {e}")
        except Exception as e:
            print_debug(f"Unexpected error: {e}")
        finally:
            stop_threads = True

        
       
    

