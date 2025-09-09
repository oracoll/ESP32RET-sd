#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

struct CAN_FRAME; // Forward declaration

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
