# main.py
from utils.serialComm import communicate_with_serial
from constants.constants import *
from utils.helperFunc import *


# TO DO: re add in handshake before starting any serial comms
if __name__ == "__main__":
    
    while True:
        user_input = input("Please Select Mode by Entering a Number. 0: Standby, 1: File Download, 2: Purge Files, 3: Log Data, 4: Manual Fin Control: ").strip().lower()
        try:
            user_input_as_int = int(user_input)
            communicate_with_serial(MODE_MSG_ARRAY[user_input_as_int])
        except ValueError:
            print("That's not a valid number!")
    