#include "sd_logger.h"

// SD Card CS pin
#define SD_CS_PIN 4

SDLogger::SDLogger() : isLogging(false), logFileNumber(0) {
}

bool SDLogger::begin() {
    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("Card failed, or not present");
        return false;
    }
    Serial.println("card initialized.");
    return true;
}

void SDLogger::startLogging() {
    if (isLogging) {
        return; // Already logging
    }

    // Find a new log file name
    char fileName[16];
    do {
        logFileNumber++;
        sprintf(fileName, "/log_%03d.csv", logFileNumber);
    } while (SD.exists(fileName));

    logFile = SD.open(fileName, FILE_WRITE);

    if (logFile) {
        Serial.print("Starting log: ");
        Serial.println(fileName);
        logFile.println("Timestamp,Bus,Type,ID,Length,Data");
        isLogging = true;
    } else {
        Serial.print("Error opening ");
        Serial.println(fileName);
    }
}

void SDLogger::stopLogging() {
    if (!isLogging) {
        return;
    }
    Serial.println("Stopping log.");
    logFile.close();
    isLogging = false;
}

void SDLogger::logCANFrame(const CAN_FRAME& frame, int bus, const char* type) {
    if (!isLogging || !logFile) {
        return;
    }

    char buffer[128];
    sprintf(buffer, "%lu,%d,%s,0x%08X,%d,%02X%02X%02X%02X%02X%02X%02X%02X",
            millis(),
            bus,
            type,
            frame.id,
            frame.length,
            frame.data.byte[0],
            frame.data.byte[1],
            frame.data.byte[2],
            frame.data.byte[3],
            frame.data.byte[4],
            frame.data.byte[5],
            frame.data.byte[6],
            frame.data.byte[7]);

    logFile.println(buffer);
}

bool SDLogger::getIsLogging() {
    return isLogging;
}
