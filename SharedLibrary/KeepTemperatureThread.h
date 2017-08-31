#pragma once
#include <Poco/Runnable.h>
#include "ArduinoTemperature.h"
#include "ConsoleLogger.h"

class KeepTemperatureThread :
    public Poco::Runnable {

    const ArduinoTemperature* device;

public:
    void run() override {

        
    }


    KeepTemperatureThread(const ArduinoTemperature* dev) : device{dev} {

    }
    ~KeepTemperatureThread();
};

