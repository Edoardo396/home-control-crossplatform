#pragma once

#include <Poco/Net/IPAddress.h> // TODO Remove
#include "IParsable.hpp"
#include "Macros.h"
#include <list>

class Device : public IParsable {

	enum class Location {
		Device,
		Server,
		Manual
	};

	enum class State {
		Off = 0,
		On,
		NotReachable,
		Reachable,
		Operating,
		Unknown
	};

	State state = State::Unknown;
	Poco::Net::IPAddress ipAddress;
	int requiredAccessLevel;
	int port;
	std::string displayName;

public:

	static std::list<Device*>* devices;

	Device(State state, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name)
		: state(state),
		  ipAddress(ip_address),
		  requiredAccessLevel(required_access_level),
		  port(port),
		  displayName(display_name) {
	}

protected:
	std::string ExecuteCommand(std::string request) const;
	std::string ExecuteCommand(std::map<std::string,std::string> request) const;


public:
	virtual std::string ParseCommand(std::string request, std::vector<std::string> parms) override;
	bool Ping() const;
	bool CheckReachability();

	virtual void SetOn();
	virtual void SetOff();
	virtual std::string GetDeviceInfo();


	Device();
	~Device();

};

