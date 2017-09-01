#pragma once
#include <string>
#include "Macros.h"

class IParsable {
public:
	virtual ~IParsable() = default;
	 IParsable(){}

	virtual std::string ParseCommand(std::string request, Dictionary parms, class User invoker) = 0;
};
