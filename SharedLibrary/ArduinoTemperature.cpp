#include "../SharedLibrary/stdafx.h"
#include "ArduinoTemperature.h"
#include "ConsoleLogger.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "User.h"
#include <chrono>
#include <string>
#include <boost/algorithm/string/erase.hpp>

void ArduinoTemperature::KeepTemperature() {
    
    try {

        while (true) {

            float temp = getInternalTemperature();

            if (temp < myTemperature - .5f) {
                this->ExecuteCommand("setFOn");
                state = State::Operating;
                ConsoleLogger::Write("Device " + this->name + " set to on", LogType::Message);
            } else if (temp >= myTemperature + .5f) {
                this->ExecuteCommand("setOff");
                state = State::Off;
                ConsoleLogger::Write("Device " + this->name + " set to off", LogType::Message);
            }
            boost::this_thread::interruption_point();
            std::this_thread::sleep_for(std::chrono::seconds(20));
            boost::this_thread::interruption_point();
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

float ArduinoTemperature::ArduinoNANTemp(const std::string& _resp) const {

    std::string resp = _resp;

    boost::erase_all(resp, "\r\n");
    

    try { return boost::lexical_cast<float>(resp); }
    catch (boost::bad_lexical_cast& e) {
       // TODO FIND A WAY SetOff();
        ConsoleLogger::Write((boost::format("There is a problem with a value retrieved from %1%, the value is %2%, the device has been set to off for precaution.") % getName() % resp).str(), LogType::Error);
        return std::numeric_limits<float>::infinity();
    }


}

float ArduinoTemperature::getInternalTemperature() const { return ArduinoNANTemp(this->ExecuteCommand("getTemp")); }

int ArduinoTemperature::getInternalHumidity() const { return ArduinoNANTemp(this->ExecuteCommand("getHumi")); }

void ArduinoTemperature::ChangeKTLocation(Location nl) {


    if (nl == Location::Manual) {
        StopKT();
        SetOff();
        return;
    }
    
    if(nl == Location::Device) {
        StopKT();
        if (!(this->ExecuteCommand(Dictionary({ {"request","setTemp"}, {"params", std::to_string(myTemperature)} })) == "true")) {
            ConsoleLogger::Write("Error while changing KTLoc for " + name + " setting to manual off.", LogType::Warning);
            this->ChangeKTLocation(Location::Manual);
            return;
        }
        this->ExecuteCommand("setOn");
    }

    if (nl == Location::Server && keepTempLocation != Location::Server)
        StartKT();
    



    this->keepTempLocation = nl;
}

void ArduinoTemperature::StartKT() {
    thread = new boost::thread(&ArduinoTemperature::KeepTemperature, this);
    ConsoleLogger::Write("KeepTemperature thread started on " + name, LogType::Message);
}

void ArduinoTemperature::StopKT() {
    thread->interrupt();
    thread->join();
    thread = nullptr;
    ConsoleLogger::Write("KeepTemperature thread stopped on " + name, LogType::Message);
}


std::string ArduinoTemperature::ParseCommand(std::string request, Dictionary parms, User invoker) {

    std::string baseResponse = Super::ParseCommand(request, parms, invoker);

    if (baseResponse != "CommandNotFound")
        return baseResponse;

    if (request == "getKTLoc") return locationStr[keepTempLocation];
    if(request == "setKTLoc") {
        this->ChangeKTLocation(Device::GetKeyByValueInMap(locationStr, parms["p0"]));
        return "true";
    }





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
