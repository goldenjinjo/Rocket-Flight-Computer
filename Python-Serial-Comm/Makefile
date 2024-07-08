# Detect the operating system
ifeq ($(OS),Windows_NT)
    PYTHONPATH := $(shell echo $(CURDIR))
    PYTHON = $(VENV)/Scripts/python
    PIP = $(VENV)/Scripts/pip
else
    PYTHONPATH := $(shell echo $(CURDIR))
    PYTHON = $(VENV)/bin/python
    PIP = $(VENV)/bin/pip
endif

export PYTHONPATH

# Define the virtual environment directory
VENV := venv

# Define the requirements file
REQS := requirements.txt

# Default target: setup the virtual environment, install dependencies, and run the main script
all: run

# Target to create the virtual environment if it doesn't exist
venv:
	@if [ ! -d "$(VENV)" ]; then \
		python -m venv $(VENV); \
	fi

# Target to install dependencies if they are not already installed
install: venv
	$(PIP) install -r $(REQS)

# Target to run tests
test: install
	$(PYTHON) -m unittest discover -s tests

# Target to run the main script
run: install
	$(PYTHON) main.py

# Target to clean the project (remove the virtual environment)
clean:
	rm -rf $(VENV)

.PHONY: all venv install test run clean
