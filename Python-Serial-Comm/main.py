# main.py

from downloadFlashData import communicate_with_serial

if __name__ == "__main__":
    user_input = input("Would you like to request a file download? (yes/no): ").strip().lower()
    if user_input == 'yes':
        communicate_with_serial()
    else:
        print("No file download request sent. Exiting program.")
