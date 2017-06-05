#pragma once

#include <Poco/Net/IPAddress.h> // TODO Remove
#include "IParsable.h"
#include "Macros.h"

ABSTRACT class Device : public IParsable {

	enum class Location {
		Device,
		Server
	};

	enum class State {
		Off = 0,
		On,
		NotReachable,
		Reachable,
		Operating,
		Unknown
	};

	State state;
	Poco::Net::IPAddress ipAddress;
	int requiredAccessLevel;
	int port;
	std::string displayName;


public:
	Device(State state, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name)
		: state(state),
		  ipAddress(ip_address),
		  requiredAccessLevel(required_access_level),
		  port(port),
		  displayName(display_name) {
	}

	// TODO ExecuteCommand
	// TODO ExecuteCommandDict

	// TODO Ping

protected:
	std::string ExecuteCommand(std::string request) const;
	std::string ExecuteCommand(std::map<std::string,std::string> request) const;


public:
	virtual std::string ParseCommand(std::string request, std::vector<std::string> parms) override;
	bool Ping() const;
	bool CheckReachability();

	virtual void SetOn();
	virtual void SetOff();


	Device();
	~Device();

};

