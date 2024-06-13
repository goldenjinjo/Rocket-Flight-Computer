# config.py
TIMEOUT_SECONDS = 180  # 3 minutes
BAUD_RATE = 115200  # Ensure this matches the flight computer's baud rate
DEBUG = True  # Set this to True for debugging

# Connection retries and backoff
RETRIES = 5
BACKOFF_FACTOR = 1.5
