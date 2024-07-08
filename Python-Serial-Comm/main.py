from utils.serialComm import communicate_with_serial
from constants.constants import *
from utils.helperFunc import *

# TO DO: re add in handshake before starting any serial comms
if __name__ == "__main__":
    
    while True:
        user_input = input(
            "Please Select Mode by Entering a Number:\n"
            "0: Standby\n"
            "1: File Download\n"
            "2: Purge Files\n"
            "3: Log Data\n"
            "4: Manual Fin Control\n"
            "5: Change Settings\n"
        ).strip().lower()
        try:
            user_input_as_int = int(user_input)
            communicate_with_serial(MODE_MSG_ARRAY[user_input_as_int])
        except ValueError:
            print("That's not a valid number!")
        except IndexError:
            print("Invalid selection. Please enter a number between 0 and 5.")
