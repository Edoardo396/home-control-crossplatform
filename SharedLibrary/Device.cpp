#include "../HomeControlController (Windows)/stdafx.h"
#include "Device.h"
#include "Poco/Net/ICMPClient.h"
#include "Command.h"

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

	} catch (std::exception& e) {
		return "false, " + std::string(e.what());
	}

	return "false";
}

bool Device::Ping() const {
	return Poco::Net::ICMPClient::pingIPv4(ipAddress.toString()) >= 1;
}

bool Device::CheckReachability() {



}

Device::Device() {
}


Device::~Device() {

}


