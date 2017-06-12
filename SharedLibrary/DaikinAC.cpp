#include "../HomeControlController (Windows)/stdafx.h"
#include "DaikinAC.h"
#include "Command.h"
#include <boost/algorithm/string.hpp>
#include <future>
#include "User.h"


void DaikinAC::PullData() {
	using std::string;

	string response = Command::ExecuteGETRequest(ipAddress, port, "aircon/get_control_info");

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

void DaikinAC::PushData() {
	std::async([this]() {
		Command::ExecutePOSTRequest(ipAddress, port, "aircon/set_control_info", { 
		{ "pow", this->state == State::Operating ? "1" : "0" },   // 4th parameter is Dictionary
		{ "mode", this->IDFromMode(this->opMode) },
		{ "stemp", std::to_string(myTemperature) },
		{ "shum", "0" },{ "f_rate", this->IDFromFRATE(this->fanSpeed) },
		{ "f_dir", this->IDFromThisFDIR() } 
		}); 
	});
}

std::string DaikinAC::ParseCommand(std::string request, Dictionary parms, User invoker) {
	return Super::ParseCommand(request, parms, invoker);
}

void DaikinAC::SetOn() {
	this->state = State::Operating;

	std::async([this]() {
		PushData();
		PullData();
	});
}

void DaikinAC::SetOff() {
	this->state = State::Off;

	std::async([this]() {
		PushData();
		PullData();
	});
}

DaikinAC::~DaikinAC() {
}
