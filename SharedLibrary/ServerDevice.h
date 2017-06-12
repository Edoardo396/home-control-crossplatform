#pragma once
#include "Device.h"

class ServerDevice : public Device {

	typedef Device Super;
	
	public:


	std::string ParseCommand(std::string request, Dictionary parms, class User invoker) override;

	void SetOn() const override;

	void SetOff() const override;

	std::string GetDeviceInfo() const override;

	ServerDevice(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state)
		: Device(name, ip_address, required_access_level, port, display_name, state) {
	}

	ServerDevice(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
		: Device(name, ip_address, required_access_level, port) {
	}

	~ServerDevice();
};
