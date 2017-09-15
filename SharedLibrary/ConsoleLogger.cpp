#pragma once
#include "ConsoleLogger.h"


std::mutex ConsoleLogger::mtx;
std::ofstream* ConsoleLogger::filestream;