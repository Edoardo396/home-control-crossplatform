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

    friend class AutomatorSyncronizer;

    template <class Key, class Value>
    [[deprecated("Use Support class instead")]]
    static Key GetKeyByValueInMap(std::map<Key, Value> map, Value val) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (it->second == val)
                return it->first;
        }

        throw std::runtime_error(std::string("Item not found"));
    }

    static std::string SerializeListDevDisp(std::vector<const Device*> vector);
	static inline std::string StringState(State s) { return statesStr[s]; }
    virtual std::string getType() const = 0;
    
    // TODO Remove in version 2
    [[deprecated("Use statesMap insted, will be removed in version 2")]]
    static Device::Location LocationByText(std::string& location);
    State getState() const { return state; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

protected:
	bool IsOK() const {
		return state == State::On || state == State::Reachable ||
			state == State::Operating;
	}

	static std::map<State, std::string> statesStr;
    static std::map<Location, std::string> locationStr;

    class DeviceAutomator* automator = nullptr;
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
        name(name),
    automator(nullptr) {
        if (state == State::Unknown) this->CheckReachability();
	}


	Device(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
		: ipAddress(ip_address),
		  requiredAccessLevel(required_access_level),
		  port(port),
		  name(name) { this->CheckReachability(); }

protected:
	std::string ExecuteCommand(std::string request, int timeout = 15) const;
	std::string ExecuteCommand(Dictionary request, int timeout = 15) const;


public:

    void AttachAutomator(DeviceAutomator* automator) { this->automator = automator; }
	virtual std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
	bool Ping() const;
	virtual bool CheckReachability();

	inline virtual void SetOn();
	inline virtual void SetOff();
	virtual std::string GetDeviceInfo() const;

	virtual std::string ToString() const;

	~Device();
};

