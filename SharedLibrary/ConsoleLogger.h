#pragma once
#include <string>
#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Macros.h"
#include <fstream>
#include <cassert>
#include <mutex>

enum class LogType {
	Automator = 0, Warning = 1, Error = 2, Message = 3
};

class ConsoleLogger {

    static std::mutex mtx;
    static std::ofstream* filestream;

public:

    static void Init(std::string file) {
        filestream = new std::ofstream(file);

        if (!filestream->good())
            ConsoleLogger::Log("Log file not found", LogType::Warning);
    }

    static void Finalize() {
        filestream->flush();
        filestream->close();
        delete filestream;
    }
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

    static inline void Log(std::string text, LogType type) {
        Write(text.c_str(), type);
    }

    static inline void Log(const char* text, LogType type) {
        Write(text, type);
    }


    [[deprecated("Use Log() instead")]]
	static void Write(std::string text, LogType type) {
		Write(text.c_str(), type);
	}

    [[deprecated("Use Log() instead")]]
	static void Write(const char* text, LogType type) {
        if (filestream == nullptr) { 
            std::cerr << "ERROR LOG FILE IS NULL\n"; 
            std::cout << "ERROR LOG FILE IS NULL\n";
            std::cin.get();
            exit(1);
        }

        mtx.lock();

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

        if (filestream->good())
            *filestream << enummsg + " " + text << std::endl;

        printf(KWHT);
        fflush(stdout);
        mtx.unlock();
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
        
        if(filestream->good()) 
            *filestream << enummsg + " " + text << std::endl;

        mtx.unlock();
#endif
	}

    static std::string LogAndReturn(std::string text, LogType type) {
        Write(text, type);
        return text;
	}


};