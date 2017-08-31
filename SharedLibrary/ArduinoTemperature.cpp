#include "../SharedLibrary/stdafx.h"
#include "ArduinoTemperature.h"
#include "ConsoleLogger.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "User.h"
#include <future>
#include <chrono>
#include "KeepTemperatureThread.h"

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

            std::this_thread::sleep_for(std::chrono::seconds(60));
            ConsoleLogger::Write("Checking internal temperature of " + this->getName() + "...", LogType::Automator);
        }
    } catch(boost::thread_interrupted& e) {
        ConsoleLogger::Write("Temp thread interrupted", LogType::Warning);
        return;
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

void ArduinoTemperature::StartKT() {
    thread = new boost::thread(&ArduinoTemperature::KeepTemperature, this);
}

void ArduinoTemperature::StopKT() {
    thread->interrupt();
    thread->join();
    thread = nullptr;
}


std::string ArduinoTemperature::ParseCommand(std::string request, Dictionary parms, User invoker) {
    return std::string("CommandNotFound");
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

void ArduinoTemperature::SetOff() {
}

std::string ArduinoTemperature::GetDeviceInfo() const {
    return std::string();
}

ArduinoTemperature::~ArduinoTemperature() {


}
