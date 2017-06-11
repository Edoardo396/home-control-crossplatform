#include "../HomeControlController (Windows)/stdafx.h"
#include "Device.h"
#include "Poco/Net/ICMPClient.h"
#include "Command.h"
#include "ConsoleLogger.h"
#include "Poco/Format.h"
#include <boost/format.hpp>

std::list<Device*>* Device::devices;

std::string Device::ExecuteCommand(std::string request) const {
	return Command::ExecuteGETRequest(ipAddress, port, request);

}

std::string Device::ExecuteCommand(std::map<std::string, std::string> request) const {
	return Command::ExecuteGETRequest(ipAddress, port, Command::GetCommandDir(request));
}

std::string Device::ParseCommand(std::string request, std::vector<std::string> parms) {

	// TODO Autmator

	try {

		if (request == "ping")
			return this->Ping() ? "true" : "false";

		if (request == "getAL")
			return std::to_string(requiredAccessLevel);


		if (request == "setOn") { this->SetOn(); return "true"; }
		if (request == "setOff") { this->SetOff(); return "true"; }

		// TODO GetDeviceInfo, GetState, getDisplayName

		if (request == "getDeviceInfo") { return this->ExecuteCommand(request); }
		if (request == "getState") { return this->ExecuteCommand(request); }
		if (request == "getDisplayName") { return this->ExecuteCommand(request); }

	} catch (std::exception& e) {
		return "false, " + std::string(e.what());
	}

	return "false";
}

bool Device::Ping() const {
	return Poco::Net::ICMPClient::pingIPv4(ipAddress.toString()) >= 1;
}

bool Device::CheckReachability() {

	try {
		auto cmd = Command::ExecuteGETRequest(ipAddress, port, Command::GetCommandDir("ping"));
	} catch(std::exception& e) {
		ConsoleLogger::Write((boost::format("Device %1% is not reachable. %2%") % name % e.what()).str(), LogType::Error);
		state = State::NotReachable;
		return false;
	}

	ConsoleLogger::Write((boost::format("Device %1% reached successfully.") % name).str(), LogType::Message);
	state = State::Reachable;
	return true;
}

void Device::SetOn() {
	this->ExecuteCommand("setOn");
}

void Device::SetOff() {
	this->ExecuteCommand("setOff");
}

std::string Device::GetDeviceInfo() {
	return this->ExecuteCommand("getDeviceInfos");
}

Device::~Device() {

}


