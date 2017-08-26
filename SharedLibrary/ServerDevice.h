#pragma once
#include "Device.h"
#include <functional>

class ServerDevice : public Device {

    typedef Device Super;
    typedef std::function<std::string(std::string, Dictionary, class User invoker, ServerDevice* sender)> CommandDelegate;
private:
    CommandDelegate on_command_delegate = nullptr;

public:


    std::string getType() const override { return "ServerDevice"; }
    std::string ParseCommand(std::string request, Dictionary parms, class User invoker) override;

    void SetOn() override;

    void SetOff() override;

    std::string GetDeviceInfo() const override;

    ServerDevice(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port,
                 const std::string& display_name, State state)
        : ServerDevice(name, ip_address, required_access_level, port, display_name, state, nullptr) {
    }

    ServerDevice(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port,
                 const std::string& display_name, State state, CommandDelegate onCommandDelegate)
        : Device(name, ip_address, required_access_level, port, display_name, state) {
        this->on_command_delegate = onCommandDelegate;
    }

    ServerDevice(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
        : Device(name, ip_address, required_access_level, port) {
    }

    void UpdateDB();

    ~ServerDevice();
};
