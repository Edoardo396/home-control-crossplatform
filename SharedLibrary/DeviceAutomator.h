#pragma once
#include "IParsable.hpp"

class DeviceAutomator : public IParsable {

    friend class Device;
    class Device* device;
    std::vector<class AutomatorAction> actions;
    bool enabled;

public:
    
    std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
    DeviceAutomator(Device* device);
    void Check();
    std::string ToString();
    ~DeviceAutomator();
};