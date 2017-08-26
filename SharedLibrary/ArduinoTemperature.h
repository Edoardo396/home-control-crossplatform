#pragma once
#include "Device.h"
#include <Poco/Thread.h>

class ArduinoTemperature : public Device {

    typedef Device Super;

    void KeepTemperature();
    float ArduinoNANTemp(const std::string& resp) const;

protected:

    float getInternalTemperature() const;
    int getInternalHumidity() const;

    Location keepTempLocation;
    bool autoStart;
    double myTemperature;

public:
    std::string getType() const override { return "ArduinoTemperature"; }

    ArduinoTemperature(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state, Location keep_temp_location, bool auto_start, double my_temperature)
        : Device(name, ip_address, required_access_level, port, display_name, state),
          keepTempLocation(keep_temp_location),
          autoStart(auto_start),
          myTemperature(my_temperature) {
    }

    ArduinoTemperature(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, Location keep_temp_location, bool auto_start, double my_temperature)
        : Device(name, ip_address, required_access_level, port),
          keepTempLocation(keep_temp_location),
          autoStart(auto_start),
          myTemperature(my_temperature) {
    }


protected:
    void StartKT() const;
    void StopKT() const;


public:
    std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
    void SetOn() override;
    void SetOff() override;
    std::string GetDeviceInfo() const override;
    ~ArduinoTemperature();
};

