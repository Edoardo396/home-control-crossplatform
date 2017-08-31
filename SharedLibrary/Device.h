#pragma once
#include <Poco/Net/IPAddress.h> // TODO Remove
#include "IParsable.hpp"
#include "Macros.h"
#include <list>

class Device : public IParsable {

public:
	enum class Location {
		Device = 0,
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

	static inline std::string StringState(State s) { return statesStr[s]; }

	/*
	ENUM(State, char, Off = 0,
		On,
		NotReachable,
		Reachable,
		Operating,
		Unknown
	);
	*/

public:

    virtual std::string getType() const = 0;
    static Device::Location LocationByText(std::string& location);
    static std::string SerializeListDevDisp(std::vector<const Device*> vector);

    State getState() const { return state; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

protected:
	bool IsOK() const {
		return state == State::On || state == State::Reachable ||
			state == State::Operating;
	}

	static std::map<State, std::string> statesStr;

	State state = State::Unknown;
	Poco::Net::IPAddress ipAddress;
	int requiredAccessLevel;
	int port;
	std::string displayName;
	std::string name;

    virtual std::string getAllInfos();

public:


	std::string getName() const { return name; }
	static std::list<Device*>* devices;

	Device(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state)
		: state(state),
		  ipAddress(ip_address),
		  requiredAccessLevel(required_access_level),
		  port(port),
		  displayName(display_name),
        name(name) {
        if (state == State::Unknown) this->CheckReachability();
	}


	Device(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
		: ipAddress(ip_address),
		  requiredAccessLevel(required_access_level),
		  port(port),
		  name(name) { this->CheckReachability(); }

protected:
	std::string ExecuteCommand(std::string request, int timeout = 15) const;
	std::string ExecuteCommand(std::map<std::string, std::string> request, int timeout = 15) const;


public:
	virtual std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
	bool Ping() const;
	virtual bool CheckReachability();

	inline virtual void SetOn();
	inline virtual void SetOff();
	virtual std::string GetDeviceInfo() const;

	virtual std::string ToString() const;

	~Device();
};
