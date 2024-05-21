#include <SPI.h>
#include <SdFat.h>

SdFat sd;
File dataFile;
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize SD card
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    Serial.println("SD card initialization failed.");
    return;
  }

  Serial.println("SD card initialized.");

  // Open the file on the SD card
  dataFile = sd.open("data.txt");
  if (!dataFile) {
    Serial.println("Could not open data.txt");
    return;
  }

  // Open the file for writing on the USB mass storage
  File usbFile = SD.open("output_data.txt", FILE_WRITE);
  if (!usbFile) {
    Serial.println("Could not open USB mass storage file.");
    dataFile.close();
    return;
  }

  // Copy data from SD card file to USB mass storage file
  while (dataFile.available()) {
    usbFile.write(dataFile.read());
  }

  // Close the files
  dataFile.close();
  usbFile.close();

  Serial.println("Data copied to USB mass storage.");
}

void loop() {
  // Nothing to do here
}
