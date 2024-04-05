  /*
 * Sketch File to Read All Data on a FLash Chip
 * Saving to CSV file is a WIP
 */

#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

#define SPI_SPEED SD_SCK_MHZ(4)
#define SD_FAT_TYPE 3 // for FAT16

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE


int chipSelect = 10;

const char* fileName = "flight data";

// Set up - check for Flash Chip
void setup() {
  if (!sd.begin(chipSelect, SPI_SPEED)) {
    Serial.println("Flash Chip Not Detected.");
  } else {
    Serial.println("Flash Chip Found.");
  }

  if (sd.exists(fileName)) {
    Serial.println("Data File Exists");
  } else {
    Serial.println("Data file not found.");
  }

  // Read Data

    if (!file.open(fileName, O_READ)) {
    sd.errorHalt("opening for read failed");
  }
  Serial.println("----READING FLASH CHIP----:");

  // read from the file until there's nothing else in it:
  int data;
  while ((data = file.read()) >= 0) {
    Serial.write(data);
  }
  // close the file:
  file.close();

  Serial.println(" ");
  Serial.println("---------------------END DATA FILE---------------------");

}



void loop()
{
 // No Loop Needed
  
}
