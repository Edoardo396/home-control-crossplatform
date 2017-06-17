#include "../SharedLibrary/stdafx.h"
#include "ArduinoUnlocker.h"
#include "User.h"
#include "Command.h"

// TODO LEGACY! UPDATE ARDUINO!
std::string ArduinoUnlocker::Unlock() const { return Command::ExecuteGETRequest(this->ipAddress, port, "unlock", 10); }

std::string ArduinoUnlocker::ParseCommand(std::string request, Dictionary parms, User invoker) {

	auto super = Super::ParseCommand(request, parms, invoker);

	if (super != "false") return super;

	try { if (request == "unlock") return this->Unlock(); }
	catch (std::exception& e) { return "false, " + std::string(e.what()); }


	return "false";
}

ArduinoUnlocker::~ArduinoUnlocker() {
}
