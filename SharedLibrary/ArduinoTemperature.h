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
    Location keepTempLocation; // Nexer change this, use ChangeKTLocation!

protected:
    float getInternalTemperature() const;
    int getInternalHumidity() const;
    bool autoStart;
    double myTemperature;

public:
    std::string getType() const override { return "ArduinoTemperature"; }
    virtual void ChangeKTLocation(Location nl);


    ArduinoTemperature(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state, Location keep_temp_location, bool auto_start, double my_temperature)
        : Device(name, ip_address, required_access_level, port, display_name, state),
          keepTempLocation(keep_temp_location),
          autoStart(auto_start),
        myTemperature(my_temperature) {

        if (this->IsOK()) this->Sync(); // Resolved with ArduinoTemperature::Sync() override, statically.
        if (auto_start && this->IsOK() && keepTempLocation == Location::Server) this->StartKT();
        if (auto_start && this->IsOK() && keep_temp_location != Location::Server) this->SetOn();
        ChangeKTLocation(keep_temp_location);

    }

    [[deprecated("use full constructor always!")]]
    ArduinoTemperature(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, Location keep_temp_location, bool auto_start, double my_temperature)
        : ArduinoTemperature(name, ip_address, required_access_level, port, "", State::Unknown, Location::Manual, false, 20.)
    {

    }


protected:
    void StartKT();
    void StopKT();
    void Sync() override;

public:
    std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
    void SetOn() override;
    void SetOff() override;
    void SetTemp(float nt);
    float getTemp() const;
    std::string GetDeviceInfo() const override;
    ~ArduinoTemperature();
};


