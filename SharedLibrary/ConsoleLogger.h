#pragma once

#include <string>
#include <iostream>
#include "Poco/DateTime.h"

enum class LogType {
	Automator = 0, Warning = 1, Error = 2, Message = 3
};

class ConsoleLogger {
public:

	static void Write(char* text, LogType type) {


		


	}


};

