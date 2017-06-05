#pragma once

#include <string>
#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "../Macros.h"

enum class LogType {
	Automator = 0, Warning = 1, Error = 2, Message = 3
};

class ConsoleLogger {

private:

	static void InitStringLogType(OUT std::string& string, LogType type) {

		auto dt = Poco::LocalDateTime();

		string = Poco::DateTimeFormatter::format(dt.timestamp(), "%e/%n/%Y %H:%M ");

		switch(type) {
		case LogType::Automator: string += "[AUTOMATOR]";  break;
		case LogType::Warning: string += "[WARNING]";  break;
		case LogType::Error: string += "[ERROR]"; break;
		case LogType::Message: string += "[MESSAGE]";  break;
		default: string = "[NR]";
		}
	}

public:

	static void Write(std::string text, LogType type) {
		Write(text.c_str(), type);
	}

	static void Write(const char* text, LogType type) {

		auto enummsg = std::string();

		InitStringLogType(OUT enummsg, type);

		// TODO Implement for linux
#ifdef LINUX
		if (type == LogType::Automator)
			std::cout << FGRN("text") << std::endl;
#endif

#ifdef WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (type == LogType::Automator)
			SetConsoleTextAttribute(hConsole, 10);
		if (type == LogType::Warning)
			SetConsoleTextAttribute(hConsole, 14);
		if (type == LogType::Error)
			SetConsoleTextAttribute(hConsole, 12);
		if (type == LogType::Message)
			SetConsoleTextAttribute(hConsole, 11);

		std::cout << enummsg + " " + text << std::endl;


		SetConsoleTextAttribute(hConsole, 7);
#endif
	}
};

