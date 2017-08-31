#pragma once
#include "Device.h"
#include <boost/thread.hpp>
#include <thread>
#include <Poco/Thread.h>
#include <future>

class ArduinoTemperature : public Device {

    friend class KeepTemperatureThread;
    typedef Device Super;
    boost::thread* thread = nullptr;

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

        if (auto_start) this->StartKT();

    }

    [[deprecated("use full constructor always!")]]
    ArduinoTemperature(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, Location keep_temp_location, bool auto_start, double my_temperature)
        : ArduinoTemperature(name, ip_address, required_access_level, port, "", State::Unknown, Location::Manual, false, 20.)
    {

    }


protected:
    void StartKT();
    void StopKT();


public:
    std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
    void SetOn() override;
    void SetOff() override;
    std::string GetDeviceInfo() const override;
    ~ArduinoTemperature();
};

