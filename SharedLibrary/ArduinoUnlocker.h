#pragma once
#include "Device.h"

class ArduinoUnlocker : public Device {

	typedef Device Super;

public:

	std::string Unlock() const;


    std::string getType() const override { return "ArduinoUnlocker"; }
	std::string ParseCommand(std::string request, Dictionary parms, class User invoker) override;



	ArduinoUnlocker(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state)
		: Device(name, ip_address, required_access_level, port, display_name, state) {
	}

	ArduinoUnlocker(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
		: Device(name, ip_address, required_access_level, port) {
	}

	~ArduinoUnlocker();
};

