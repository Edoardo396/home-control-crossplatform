#include "../HomeControlController (Windows)/stdafx.h"
#include "Device.h"
#include "Poco/Net/ICMPClient.h"
#include "../Support/Command.h"
#include "../Support/ConsoleLogger.h"
#include "Poco/Format.h"


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
		ConsoleLogger::Write(Poco::format("Device %d is not reachable anymore. %d", displayName, e.what()), LogType::Error);
		state = State::NotReachable;
		return false;
	}

	ConsoleLogger::Write(Poco::format("Device %d reached successfully.", displayName), LogType::Error);
	state = State::Reachable;
	return true;
}

void Device::SetOn() {
	this->ExecuteCommand("setOn");
}

void Device::SetOff() {
	this->ExecuteCommand("setOff");
}

Device::Device() {
}


Device::~Device() {

}


