#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

#define SPI_SPEED SD_SCK_MHZ(4)
#define SD_FAT_TYPE 3 // for FAT16


const int8_t DISABLE_CHIP_SELECT = -1;

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

// Store error strings in flash to save RAM.
#define error(s) sd.errorHalt(&Serial, F(s))
char line[40];
int chipSelect = 10;

// Check for extra characters in field or find minus sign.
char* skipSpace(char* str) {
  while (isspace(*str)) str++;
  return str;
}

void setup() {
  // put your setup code here, to run once:
  if (!sd.begin(chipSelect, SPI_SPEED)) {
    Serial.println("You suck");
  } else {
    Serial.println("You do not suck");
  }


   // Remove any existing file.
  if (sd.exists("ReadCsvDemo.csv")) {
    sd.remove("ReadCsvDemo.csv");
  }
  // Create the file.
  if (!file.open("ReadCsvDemo.csv", FILE_WRITE)) {
    error("open failed");
  }
  // Write test data.
  file.print(F(
    "abc,123,456,7.89\r\n"
    "def,-321,654,-9.87\r\n"
    "ghi,333,0xff,5.55"));

  // Rewind file for read.
  file.rewind();

  while (file.available()) {
    int n = file.fgets(line, sizeof(line));
    if (n <= 0) {
      Serial.println("fgets failed");
    }
    if (line[n-1] != '\n' && n == (sizeof(line) - 1)) {
      Serial.println("line too long");
    }
    if (!parseLine(line)) {
      Serial.println("parseLine failed");
    }
    Serial.println();
  }
  file.close();
  Serial.println(F("Done"));


  file.open("test.txt", FILE_WRITE);
  file.close();
}

void loop() {
  // put your main code here, to run repeatedly:


}


bool parseLine(char* str) {
  char* ptr;

  // Set strtok start of line.
  str = strtok(str, ",");
  if (!str) return false;

  // Print text field.
  Serial.println(str);

  // Subsequent calls to strtok expects a null pointer.
  str = strtok(nullptr, ",");
  if (!str) return false;

  // Convert string to long integer.
  int32_t i32 = strtol(str, &ptr, 0);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(i32);

  str = strtok(nullptr, ",");
  if (!str) return false;

  // strtoul accepts a leading minus with unexpected results.
  if (*skipSpace(str) == '-') return false;

  // Convert string to unsigned long integer.
  uint32_t u32 = strtoul(str, &ptr, 0);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(u32);

  str = strtok(nullptr, ",");
  if (!str) return false;

  // Convert string to double.
  double d = strtod(str, &ptr);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(d);

  // Check for extra fields.
  return strtok(nullptr, ",") == nullptr;
}
