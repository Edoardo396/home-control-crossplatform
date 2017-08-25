// HomeControlController (Windows).cpp : Defines the entry point for the console application.
//

#include "../SharedLibrary/stdafx.h"
#include "../SharedLibrary/MyServerApplication.h"
#include "../SharedLibrary/ConsoleLogger.h"

int main(int argc, char** argv) {

	MyServerApplication app("C:\\Users\\edoardo.fullin\\Desktop\\Database\\HomeControlDB.xml");
 
	ConsoleLogger::Write("Starting Application...", LogType::Message);

	return app.run(argc, argv);
}

