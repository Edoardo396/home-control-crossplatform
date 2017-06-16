#include "../SharedLibrary/stdafx.h"
#include "ArduinoTemperature.h"
#include "ConsoleLogger.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <thread>
#include <chrono>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/ThreadTarget.h>

void ArduinoTemperature::KeepTemperature() {
    try {

        while (true) {

            if (getInternalTemperature() < myTemperature - .5f) {
                this->ExecuteCommand("setFOn");
                state = State::Operating;
            } else if (getInternalTemperature() >= myTemperature + .5f) {
                this->ExecuteCommand("setOff");
                state = State::Off;
            }

            std::this_thread::sleep_for(std::chrono::seconds(10));
            ConsoleLogger::Write("Checking internal temperature...", LogType::Automator);
        }
    } catch (std::exception& e) {
        ConsoleLogger::Write(e.what(), LogType::Error);
        SetOff();
    }
}

float ArduinoTemperature::ArduinoNANTemp(const std::string& resp) const {

    using namespace boost;

    try { return lexical_cast<float>(resp); }
    catch (bad_lexical_cast& e) {
       // TODO FIND A WAY SetOff();
        ConsoleLogger::Write((format("There is a problem with a value retrieved from %1%, the value is %2%, the device has been set to off for precaution.") % getName() % resp).str(), LogType::Error);
        return std::numeric_limits<float>::infinity();
    }


}

float ArduinoTemperature::getInternalTemperature() const { return ArduinoNANTemp(this->ExecuteCommand("getTemp")); }

int ArduinoTemperature::getInternalHumidity() const { return ArduinoNANTemp(this->ExecuteCommand("getHumi")); }

void ArduinoTemperature::StartKT() const {


}

void ArduinoTemperature::SetOn() {
    switch (keepTempLocation) {
    case Location::Device:
        StopKT();
        this->ExecuteCommand("setOn");
        break;
    case Location::Server:
        StartKT();
        SetOff();
        break;
    case Location::Manual:
        this->ExecuteCommand("setFOn");
        break;
    default: ;
    }
}

ArduinoTemperature::~ArduinoTemperature() {
}
