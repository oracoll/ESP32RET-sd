#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "can_manager.h" // For CAN_FRAME

class SDLogger
{
public:
    SDLogger();
    bool begin();
    void startLogging();
    void stopLogging();
    void logCANFrame(const CAN_FRAME& frame, int bus, const char* type);
    bool getIsLogging();

private:
    bool isLogging;
    File logFile;
    int logFileNumber;
};
