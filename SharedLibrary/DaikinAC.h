#pragma once
#include "Device.h"

class DaikinAC : public Device {
public:

	enum class Mode { Auto, Dry, Cold, Hot, Fan };
	enum class FanSpeed { Auto, Silence, L1, L2, L3, L4, L5 };

private:
	typedef Device Super;

	static std::map<Mode, std::string> modeStr;

protected:
    std::string getAllInfos() override;
    void Sync() override;

    private:
	static std::map<FanSpeed, std::string> fanSpeedStr;

	float internalTemperature, externalTemperature, myTemperature;
	bool swingX, swingY;
	FanSpeed fanSpeed;
	Mode opMode;

public:
    std::string getType() const override { return "DaikinAC"; }

private:
#pragma region Converters
	std::string IDFromThisFDIR() const {
		if (swingX && swingY)
			return "3";
		if (swingY && !swingX)
			return "1";
		if (!swingY && swingX)
			return "2";
		return "0";
	}

	std::string IDFromFRATE(FanSpeed fanSpeed) const {
		switch (fanSpeed) {
		case FanSpeed::Auto:
			return "A";
		case FanSpeed::Silence:
			return "B";
		case FanSpeed::L1:
			return "3";
		case FanSpeed::L2:
			return "4";
		case FanSpeed::L3:
			return "5";
		case FanSpeed::L4:
			return "6";
		case FanSpeed::L5:
			return "7";
		default:
			return "A";
		}
	}

	std::string IDFromMode(Mode opMode) const {
		switch (opMode) {
		case Mode::Dry:
			return "2";
		case Mode::Cold:
			return "3";
		case Mode::Hot:
			return "4";
		case Mode::Fan:
			return "6";
		default:
			return "0";
		}
	}

	void SetSwingDir(std::string s) {
		switch (s[0]) {
		case '0':
			this->swingX = false;
			this->swingY = false;
			break;
		case '1':
			this->swingX = false;
			this->swingY = true;
			break;
		case '2':
			this->swingX = true;
			this->swingY = false;
			break;
		case '3':
			this->swingX = true;
			this->swingY = true;
			break;
		}
	}

	FanSpeed FanFromID(std::string s) {
		switch (s[0]) {
		case 'A':
			return FanSpeed::Auto;
		case 'B':
			return FanSpeed::Silence;
		case '3':
			return FanSpeed::L1;
		case '4':
			return FanSpeed::L2;
		case '5':
			return FanSpeed::L3;
		case '6':
			return FanSpeed::L4;
		case '7':
			return FanSpeed::L5;
		default:
			return FanSpeed::Auto;
		}
	}

	Mode ModeFromID(int parse) {
		switch (parse) {
		case 2:
			return Mode::Dry;
		case 3:
			return Mode::Cold;
		case 4:
			return Mode::Hot;
		case 6:
			return Mode::Fan;
		default:
			return Mode::Auto;
		}
	}
#pragma endregion

	void PullControlData();
	void PullSensorData();

protected:

	void PullData();
	void PushData();

public:

	DaikinAC(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port, const std::string& display_name, State state)
		: Device(name, ip_address, required_access_level, port, display_name, state) { if (this->IsOK()) Sync(); }

	DaikinAC(const std::string& name, const Poco::Net::IPAddress& ip_address, int required_access_level, int port)
		: Device(name, ip_address, required_access_level, port) { if (this->IsOK()) Sync(); }

	std::string ParseCommand(std::string request, Dictionary parms, User invoker) override;
	inline void SetOn() override;
	inline void SetOff() override;

	~DaikinAC();
};
