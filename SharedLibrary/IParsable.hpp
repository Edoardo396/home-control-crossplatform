#pragma once
#include <string>

class IParsable {
public:
	virtual ~IParsable() = default;
	 IParsable(){}

	virtual std::string ParseCommand(std::string request, std::vector<std::string> parms) = 0;

};
