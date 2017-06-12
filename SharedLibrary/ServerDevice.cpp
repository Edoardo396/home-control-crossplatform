#include "../HomeControlController (Windows)/stdafx.h"
#include "ServerDevice.h"
#include "ConsoleLogger.h"
#include "User.h"

std::string ServerDevice::ParseCommand(std::string request, Dictionary parms, User invoker) {
	return Super::ParseCommand(request, parms, invoker);
}

void ServerDevice::SetOn() const {
	ConsoleLogger::Write("SetOn has no sense on ServerDevice.", LogType::Warning);
}

void ServerDevice::SetOff()const {
	ConsoleLogger::Write("Shutting Down", LogType::Message);
	system("shutdown -s -f -t 0 -c \"Shutdown by HomeControl\"");
}

std::string ServerDevice::GetDeviceInfo() const {
	return "ServerSelf;RaspBerry Pi 3;::1";
}

ServerDevice::~ServerDevice() {

}
