#include "sd_logger.h"
#include "esp32_can.h" // For CAN_FRAME definition

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
        logFile.flush(); // Force write header to card
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
    logFile.close(); // close() also flushes the buffer
    isLogging = false;
}

void SDLogger::logCANFrame(const CAN_FRAME& frame, int bus, const char* type) {
    if (!isLogging || !logFile) {
        return;
    }

    char buffer[128];
    int offset = sprintf(buffer, "%lu,%d,%s,0x%X,%d,",
            millis(),
            bus,
            type,
            frame.id,
            frame.length);

    for (int i = 0; i < frame.length; i++) {
        offset += sprintf(buffer + offset, "0x%02X ", frame.data.byte[i]);
    }

    logFile.println(buffer);
    logFile.flush(); // Force write frame to card
}

bool SDLogger::getIsLogging() {
    return isLogging;
}
