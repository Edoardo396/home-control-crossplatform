#include "../SharedLibrary/stdafx.h"
#include "ArduinoTemperature.h"
#include "ConsoleLogger.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "User.h"
#include <chrono>
#include "Support.h"
#include <string>
#include <boost/algorithm/string/erase.hpp>

void ArduinoTemperature::KeepTemperature() {
    
    try {

        while (true) {
            ConsoleLogger::Write("Checking internal temperature of " + this->getName() + "...", LogType::Message);
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
            for(short i = 0; i< 6 ;++i) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
                boost::this_thread::interruption_point();
            }
          
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
        if(keepTempLocation == Location::Server) StopKT();
        this->ExecuteCommand({ { "request", "setMyTemp" },{ "params", "-1.0" } });
        SetOff();
    }
    
    if(nl == Location::Device) {
        if (this->ExecuteCommand(Dictionary({ {"request","setMyTemp"}, {"params", std::to_string(myTemperature)} })) != "true") {
            ConsoleLogger::Write("Error while changing KTLoc for " + name + " setting to manual off.", LogType::Warning);
            this->ChangeKTLocation(Location::Manual);
            return;
        }
        StopKT();
    }

    if (nl == Location::Server && keepTempLocation != Location::Server) {
        ChangeKTLocation(Location::Manual);
        StartKT();
    }

    this->keepTempLocation = nl;
}

void ArduinoTemperature::StartKT() {
    if(thread != nullptr) {
        thread->interrupt();
        thread->join();
        thread = nullptr;
    }
    thread = new boost::thread(&ArduinoTemperature::KeepTemperature, this);
    ConsoleLogger::Write("KeepTemperature thread started on " + name, LogType::Message);
}

void ArduinoTemperature::StopKT() {
    if (thread == nullptr) return;
    thread->interrupt();
    thread->join();
    thread = nullptr;
    ConsoleLogger::Write("KeepTemperature thread stopped on " + name, LogType::Message);
}

void ArduinoTemperature::Sync() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    this->ExecuteCommand({ {"request", "setMyTemp"}, {"params", keepTempLocation == Location::Device ? std::to_string(myTemperature) : "-1.0"} });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ConsoleLogger::Log(name + " synced.", LogType::Message);
}


std::string ArduinoTemperature::ParseCommand(std::string request, Dictionary parms, User invoker) {

    std::string baseResponse = Super::ParseCommand(request, parms, invoker);

    if (baseResponse != "CommandNotFound")
        return baseResponse;

    if (request == "getKTLoc") return locationStr[keepTempLocation];

    if(request == "setKTLoc") {
        this->ChangeKTLocation(Support::GetKeyByValueInMap(locationStr, parms["p0"]));
        return "true";
    }

    if (request == "getTemp"
        || request == "ping"
        || request == "getHumi" 
        || request == "getHIndex"
        || request == "getState" ) return this->ExecuteCommand(request);

    if(request == "setMyTemp") {
        
        try { this->SetTemp(boost::lexical_cast<float>(parms["p0"])); return "true"; } catch (...) { return "false"; }
    }

    if (request == "getMyTemp") {
        try { return std::to_string(this->getTemp()); } catch (...) { return "false"; }
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
    switch (keepTempLocation) {
    case Location::Device:
        this->ExecuteCommand("setOff");
        break;
    case Location::Server:
        StopKT();
        SetOff();
        break;
    case Location::Manual:
        this->ExecuteCommand("setOff");
        break;
    default:;
    }
}

void ArduinoTemperature::SetTemp(float nt) {

    if(keepTempLocation != Location::Device) {
        std::async([nt, this]() {
            this->ExecuteCommand({ {"request", "setMyTemp"}, {"params", "-1.0"} });
        });
        this->myTemperature = nt;
    }

    if (keepTempLocation == Location::Device) {  
        this->ExecuteCommand({ { "request", "setMyTemp" },{ "params", std::to_string(nt) } });
        this->myTemperature = nt;
    }

}

float ArduinoTemperature::getTemp() const {
    if (keepTempLocation == Location::Server || keepTempLocation == Location::Manual)
        return myTemperature;

    return boost::lexical_cast<float>(this->ExecuteCommand("getMyTemp"));
}

std::string ArduinoTemperature::GetDeviceInfo() const {
    return this->ExecuteCommand("getDeviceInfos");
}

ArduinoTemperature::~ArduinoTemperature() {
    this->SetOff();
    // this->Sync();
}
