#include "../SharedLibrary/stdafx.h"
#include "../SharedLibrary/MyServerApplication.h"
#include "../SharedLibrary/ConsoleLogger.h"

int main(int argc, char** argv) {
    
    MyServerApplication app(*(argv + 1));

    ConsoleLogger::Init(argc > 2 ? argv[2] : "");

    ConsoleLogger::Write("Starting Application...", LogType::Message);

    return app.run(argc, argv);
}