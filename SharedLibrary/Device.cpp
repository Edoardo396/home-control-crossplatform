#include "../SharedLibrary/stdafx.h"
#include "Device.h"
#include "Poco/Net/ICMPClient.h"
#include "Command.h"
#include "ConsoleLogger.h"
#include "Poco/Format.h"
#include <boost/format.hpp>
#include "User.h"
#include <typeinfo>

std::list<Device*>* Device::devices;
std::map<Device::State, std::string> Device::statesStr = {{State::Off, "Off"},{State::On, "On"},{State::NotReachable, "NotReachable"},{State::Reachable, "Reachable"},{State::Operating, "Operating"},{State::Unknown, "Unknown"}};
std::map<Device::Location, std::string> Device::locationStr = { {Device::Location::Device, "Device"},{ Device::Location::Manual, "Manual" } ,{ Device::Location::Server, "Server" } };

std::string Device::SerializeListDevDisp(std::vector<const Device*> vector) {
    auto rtn = std::string();

    for (const Device* it : vector) {
        rtn += it->getName() + "-";
        rtn += it->getType() + ";";
    }

    rtn.erase(rtn.size() - 1, 1);

    return rtn;
}

std::string Device::getAllInfos() {
    return this->ToString();
}

std::string Device::ExecuteCommand(std::string request, int timeout) const { return Command::ExecuteGETRequest(ipAddress, port, request, timeout, false); }

std::string Device::ExecuteCommand(std::map<std::string, std::string> request, int timeout) const { return Command::ExecuteGETRequest(ipAddress, port, Command::GetCommandDir(request), timeout); }

std::string Device::ParseCommand(std::string request, Dictionary parms, User invoker) {

    // TODO Autmator

    try {

        if (request == "ping")
            return this->Ping() ? "true" : "false";

        if (request == "getAL")
            return std::to_string(requiredAccessLevel);


        if (request == "setOn") {
            this->SetOn();
            return "true";
        }
        if (request == "setOff") {
            this->SetOff();
            return "true";
        }

        // TODO GetDeviceInfo, GetState, getDisplayName

        if (request == "getDeviceInfo") { return this->ExecuteCommand(request); }
        if (request == "getState") { return this->ExecuteCommand(request); }
        if (request == "getDisplayName") { return displayName; }
        if (request == "getAllInfos") { return this->getAllInfos(); }


    }
    catch (std::exception& e) { return "false, " + std::string(e.what()); }

    return "CommandNotFound";
}

bool Device::Ping() const { return Poco::Net::ICMPClient::pingIPv4(ipAddress.toString()) >= 1; }

bool Device::CheckReachability() {

    try {
        auto cmd = this->ExecuteCommand("ping", 10);

    } catch (std::exception& e) {
        ConsoleLogger::Write((boost::format("Device %1% is not reachable. %2%") % name % e.what()).str(), LogType::Error);
        state = State::NotReachable;
        return false;
    }

    ConsoleLogger::Write((boost::format("Device %1% reached successfully.") % name).str(), LogType::Message);
    state = State::Reachable;
    return true;
}

void Device::SetOn() { this->ExecuteCommand("setOn"); state = State::Operating; }

void Device::SetOff() { this->ExecuteCommand("setOff"); state = State::Off; }

std::string Device::GetDeviceInfo() const { return this->ExecuteCommand("getDeviceInfos"); }

std::string Device::ToString() const { return (boost::format("%1% %2% %3% %4% %5%") % name % displayName % ipAddress.toString() % port % statesStr[state]).str(); }

/*
template <class Key, class Value>
Key Device::GetKeyByValueInMap(std::map<Key, Value> map, Value val) 
*/

Device::~Device() {

}

// TODO Remove in version 2
[[deprecated("Use statesMap insted, will be removed in version 2")]]
Device::Location Device::LocationByText(std::string& location) {

    if (location == "Manual")
        return Location::Manual;
    if (location == "Device")
        return Location::Device;
    if (location == "Server")
        return Location::Server;

    throw std::runtime_error("Location error.");
}