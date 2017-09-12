#pragma once
#include "IParsable.hpp"
#include "AutomatorAction.h"

class DeviceAutomator : public IParsable {

    friend class Device;
    class Device* device;
    std::vector<class AutomatorAction> actions;
    bool enabled;

public:
    
    std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
    DeviceAutomator(Device* device) : DeviceAutomator(device, std::vector<AutomatorAction>(), true) {}

    DeviceAutomator(Device* device, const std::vector<AutomatorAction>& automator_actions, bool enabled)
        : device(device),
          actions(automator_actions),
          enabled(enabled) {
    }

    void Check();
    std::string ToString();
    ~DeviceAutomator();
};