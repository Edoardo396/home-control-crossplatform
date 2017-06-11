#pragma once
#include "Device.h"



class ArduinoUnlocker : public Device {

	typedef Device Super;

public:

	std::string Unlock() const;

	std::string ParseCommand(std::string request, std::vector<std::string> parms) override;

	ArduinoUnlocker() : Super() {}
	~ArduinoUnlocker();
};

