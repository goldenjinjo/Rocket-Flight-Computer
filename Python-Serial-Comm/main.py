# main.py

from downloadFlashData import communicate_with_serial
from serialComm import *
from constants import *


# TO DO: restructure this to select modes or whatever
if __name__ == "__main__":
    user_input = input("Would you like to request a file download? (yes/no): ").strip().lower()
    if user_input == 'yes':
        communicate_with_serial("REQUEST_FILE_DOWNLOAD")
    elif user_input == 'no':
        communicate_with_serial(GO_TO_READ)
    else:
        print("No file download request sent. Exiting program.")
