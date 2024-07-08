import sys
import time
import os
import zlib
from constants.constants import *
from config.config import *
from utils.helperFunc import *

def ensure_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def configure_directory():
    # Ensure the main output directory exists
    script_directory = os.path.dirname(os.path.abspath(__file__))
    output_directory = os.path.join(script_directory, OUTPUT_DIRECTORY)
    ensure_directory(output_directory)

    # Ensure the misc folder exists
    misc_directory = os.path.join(output_directory, MISC_FOLDER)
    ensure_directory(misc_directory)

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

def download_flash_data(ser):
    
    configure_directory()
    
    try:
        # Clear the serial buffer
        ser.reset_input_buffer()

        # Send handshake message and wait for acknowledgment
        # send_handshake(ser)
        # start_time = time.time()
        # while True:
        #     if ser.in_waiting > 0:
        #         response = read_from_serial(ser)
        #         print_debug(f"Received response: {response}")
        #         if response == ACK_MESSAGE:
        #             print_debug(f"Received acknowledgment: {ACK_MESSAGE}")
        #             break
        #     if time.time() - start_time > TIMEOUT_SECONDS:
        #         print_debug("Handshake timed out. Exiting...")
        #         sys.exit()

        while True:
            file_name_received = False
            file_name = ""
            checksum = 0

            while True:
                if ser.in_waiting > 0:
                    response = read_from_serial(ser)
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
                        write_to_serial(ser, ALL_FILES_SENT_ACK)
                        time.sleep(1)
                        write_to_serial(ser, GO_TO_STANDBY)
                        sys.exit()

            if file_name_received:
                print(file_name)
                file_crc = 0
                output_file_path = sort_file(file_name)

                # Does not write to file if it is found to already exist
                if os.path.exists(output_file_path):
                    print_debug(f"File {file_name} already exists. Skipping download.")
                    write_to_serial(ser, FILE_COPY_MESSAGE)
                else:
                    # Open file for writing
                    with open(output_file_path, 'w') as f:
                        print_debug(f"Writing data to {output_file_path}")
                        crc = 0
                        while True:
                            # Read data from serial port
                            data = read_from_serial(ser)  # Decode bytes to string
                            print_debug(f"Received data: {data}")
                            if data == END_OF_TRANSMISSION_MESSAGE.strip():
                                print_debug(f"End of transmission for {file_name} received.")
                                write_to_serial(ser, END_OF_TRANSMISSION_ACK)
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