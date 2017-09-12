#include "../SharedLibrary/stdafx.h"
#include "DeviceAutomator.h"
#include "User.h"
#include "AutomatorAction.h"
#include "Device.h"
#include <boost/format.hpp>
#include "ConsoleLogger.h"
#include <string>
#include <Poco/XML/XML.h>

std::string DeviceAutomator::ParseCommand(std::string request, Dictionary parms, User invoker) {
    return "false";
}


void DeviceAutomator::Check() {
    for (auto action : actions){
        if(!action.isActive()) continue;

        if(device->ParseCommand(action.command, Dictionary(), User(-3, "Automator", "", 100)) == "false") {
            ConsoleLogger::Log((boost::format("Error executing %1% on %2%") % action.command % device->getName()).str(), LogType::Error);
            continue;
        }

        action.enabled = false;
        ConsoleLogger::Log((boost::format("Executed %1% on %2%") % action.command % device->getName()).str(), LogType::Automator);
    }
}

std::string DeviceAutomator::ToString() {
    using std::string;
    string s = "";

    for (auto a : actions) {
        s += a.ToString() + "|";
    }

    std::remove(s.end() - 2, s.end(), '|');
    return s;
}

DeviceAutomator::~DeviceAutomator() {
}
