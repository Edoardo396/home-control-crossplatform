#include <cstdio>
#include "../SharedLibrary/MyServerApplication.h"
#include "../SharedLibrary/ConsoleLogger.h"

int main(int argc, char** argv) {
    
    MyServerApplication app("/root/HomeControlDB.xml");

    ConsoleLogger::Write("Starting Application...", LogType::Message);

    return app.run(argc, argv);

    return 0;
}