#include "../HomeControlController (Windows)/stdafx.h"
#include "ArduinoUnlocker.h"


std::string ArduinoUnlocker::Unlock() const { return this->ExecuteCommand("unlock"); }

std::string ArduinoUnlocker::ParseCommand(std::string request, std::vector<std::string> parms) {

	auto super = Super::ParseCommand(request, parms);

	if (super != "false") return super;

	try { if (request == "unlock") return this->Unlock(); }
	catch (std::exception& e) { return "false, " + std::string(e.what()); }


	return "false";
}

ArduinoUnlocker::~ArduinoUnlocker() {
}
