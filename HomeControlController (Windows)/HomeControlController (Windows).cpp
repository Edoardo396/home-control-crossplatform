// HomeControlController (Windows).cpp : Defines the entry point for the console application.
//

#include "../HomeControlController (Windows)/stdafx.h"
#include "../SharedLibrary/MyServerApplication.h"
#include "../SharedLibrary/Support/ConsoleLogger.h"

int main(int argc, char** argv) {
	MyServerApplication app;

	ConsoleLogger::Write("Starting Application...", LogType::Message);

	return app.run(argc, argv);
}

