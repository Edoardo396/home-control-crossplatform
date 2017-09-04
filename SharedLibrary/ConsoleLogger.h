#pragma once
#include <string>
#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Macros.h"

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

        std::string mess = enummsg + " " + text;

        if (type == LogType::Automator)
            printf("%s%s\n", KGRN, mess.c_str());
        if (type == LogType::Warning)
            printf("%s%s\n", KYEL, mess.c_str());
        if (type == LogType::Error)
            printf("%s%s\n", KRED, mess.c_str());
        if (type == LogType::Message)
            printf("%s%s\n", KCYN, mess.c_str());


        printf(KWHT);
        fflush(stdout);
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
#endif
	}
};

