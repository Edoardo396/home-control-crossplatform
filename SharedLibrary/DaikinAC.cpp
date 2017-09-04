#include "../SharedLibrary/stdafx.h"
#include "DaikinAC.h"
#include "Command.h"
#include <map>
#include <boost/algorithm/string.hpp>
#include <future>
#include "User.h"
#include "Macros.h"
#include "Support.h"
#include <boost/format.hpp>
#include <algorithm>

std::map<DaikinAC::Mode, std::string> DaikinAC::modeStr = {{DaikinAC::Mode::Auto, "Auto"},
    {DaikinAC::Mode::Dry, "Dry"},
    {DaikinAC::Mode::Cold, "Cold"},
    {DaikinAC::Mode::Hot, "Hot"},
    {DaikinAC::Mode::Fan, "Fan"}};

std::map<DaikinAC::FanSpeed, std::string> DaikinAC::fanSpeedStr = {{DaikinAC::FanSpeed::Auto, "Auto"},{DaikinAC::FanSpeed::Silence, "Silence"},{DaikinAC::FanSpeed::L1, "L1"},{DaikinAC::FanSpeed::L2, "L2"},{DaikinAC::FanSpeed::L3, "L3"},{DaikinAC::FanSpeed::L4, "L4"},{DaikinAC::FanSpeed::L5, "L5"},};

std::string DaikinAC::getAllInfos() {

    auto rtn = std::string();

    rtn += BOOLTOSTR(state == State::Operating) + ";";
    rtn += modeStr[opMode] + ";";
    rtn += fanSpeedStr[fanSpeed] + ";";
    rtn += BOOLTOSTR(swingX) + ";";
    rtn += BOOLTOSTR(swingY) + ";";
    rtn += FLOATTOSTR(myTemperature) + ";";
    rtn += FLOATTOSTR(internalTemperature) + ";";
    rtn += FLOATTOSTR(externalTemperature) + ";";
    rtn += displayName;

    return rtn;
}

void DaikinAC::PullControlData() {
    using std::string;

    string response = Command::ExecuteGETRequest(ipAddress, port, "aircon/get_control_info", 10, true);

    auto dict = Dictionary();

    std::vector<string> strs;
    boost::split(strs, response, boost::is_any_of(",="));

    for (int i = 0; i < strs.size() - 1; i += 2)
        dict.insert(std::pair<string, string>(strs[i], strs[i + 1]));

    this->state = dict["pow"] == "1" ? State::Operating : State::Off;
    this->opMode = ModeFromID(std::stoi(dict["mode"]));
    this->myTemperature = dict["stemp"] != "M" ? std::stof(dict["stemp"]) : -1.f;
    this->fanSpeed = FanFromID(dict["f_rate"]);
    this->SetSwingDir(dict["f_dir"]);
}

void DaikinAC::PullSensorData() {
    using std::string;

    string response = Command::ExecuteGETRequest(ipAddress, port, "aircon/get_sensor_info", 10, true);

    auto dict = Dictionary();

    std::vector<string> strs;
    boost::split(strs, response, boost::is_any_of(",="));

    for (int i = 0; i < strs.size() - 1; i += 2)
        dict.insert(std::pair<string, string>(strs[i], strs[i + 1]));

    this->internalTemperature = stof(dict["htemp"]);
    this->externalTemperature = stof(dict["otemp"]);
}

void DaikinAC::PullData() {
    PullControlData();
    PullSensorData();
}

void DaikinAC::PushData() {
    Command::ExecutePOSTRequest(ipAddress, port, "aircon/set_control_info", {
                                    {"pow", this->state == State::Operating ? "1" : "0"}, // 4th parameter is Dictionary
                                    {"mode", this->IDFromMode(this->opMode)},
                                    {"stemp", boost::str(boost::format("%d") % myTemperature)},
                                    {"shum", "0"},
                                    {"f_rate", this->IDFromFRATE(this->fanSpeed)},
                                    {"f_dir", this->IDFromThisFDIR()}
                                });
}


std::string DaikinAC::ParseCommand(std::string request, Dictionary parms, User invoker) {
    auto baseResponse = Super::ParseCommand(request, parms, invoker);

    this->PullData();

    if (baseResponse != "CommandNotFound")
        return baseResponse;

    if (request == "getMyTemp") return boost::str(boost::format("%d") % myTemperature);
    if (request == "getOpMode") return this->modeStr[opMode];
    if (request == "getFanSpeed") return this->fanSpeedStr[fanSpeed];
    if (request == "getSwingX") return std::to_string(this->swingX); // TODO USE BITWISE
    if (request == "getSwingY") return std::to_string(this->swingY);
    if (request == "getTemp") return FLOATTOSTR(internalTemperature);
    if (request == "getExtTemp") return FLOATTOSTR(externalTemperature);

    if(request == "update") {
        PushData();
        PullData();
        return "true";
    }

    if (request == "setTemp") {
        this->myTemperature = std::stof(parms["p0"]);
        PushData(); // TODO check if switch to another thread worth
        return "true";
    }

    if (request == "setSwingX") {
        this->swingX = parms["p0"] == "1";
        PushData();
        return "true";
    }

    if (request == "setSwingY") {
        this->swingY = parms["p0"] == "1";
        PushData();
        return "true";
    }

    if (request == "setOpMode") {
        this->opMode = Support::GetKeyByValueInMap(modeStr, parms["p0"]);
        PushData();
        return "true";
    }

    if (request == "setFanSpeed") {
        this->fanSpeed = Support::GetKeyByValueInMap(fanSpeedStr, parms["p0"]);
        PushData();
        return "true";
    }

    return "false";
}

void DaikinAC::SetOn() {
    this->state = State::Operating;

    PushData();

    std::async([this]() { PullData(); });
}

void DaikinAC::SetOff() {
    this->state = State::Off;

    PushData();

    std::async([this]() { PullData(); });
}





DaikinAC::~DaikinAC() {
}
