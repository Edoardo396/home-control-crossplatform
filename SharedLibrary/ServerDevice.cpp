#include "../SharedLibrary/stdafx.h"
#include "ServerDevice.h"
#include "ConsoleLogger.h"
#include "User.h"
#include <Poco/Util/ServerApplication.h>
#include "MyServerApplication.h"
#include "ConsoleLogger.h"
#include <boost/timer/timer.hpp>
#include <boost/format.hpp>
#include <boost/thread/v2/thread.hpp>
#include <thread>

#ifdef LINUX    
#include <linux/reboot.h>
#include <sys/reboot.h>
#endif


std::string ServerDevice::ParseCommand(std::string request, Dictionary parms, User invoker) {
	
    if (on_command_delegate != nullptr)
        return on_command_delegate(request, parms, invoker, this);

    return "NoDelegate";
}

void ServerDevice::SetOn() {
	ConsoleLogger::Write("SetOn has no sense on ServerDevice.", LogType::Warning);
}

void ServerDevice::SetOff() {
	ConsoleLogger::Write("Shutting Down", LogType::Message);

#ifdef LINUX
    sync();
    reboot(LINUX_REBOOT_CMD_POWER_OFF);
    return;
#endif

#ifdef WIN32
	system("shutdown /s /f /t 0 /c \"Shutdown by HomeControl\"");
    return;
#endif

    ConsoleLogger::Write("Could not shutdown, check the compilation directives.", LogType::Error);
}

std::string ServerDevice::GetDeviceInfo() const {
	return "ServerSelf;RaspBerry Pi 3;::1";
}

void ServerDevice::UpdateDB() {

//    boost::timer::cpu_timer timer;
//     timer.start();
#ifdef WIN32
    system("cls");
#endif
    ConsoleLogger::Write("Starting to update db...", LogType::Message);

    ConsoleLogger::Write("Emptying old structures...", LogType::Message);
    Device::devices->erase(Device::devices->begin(), Device::devices->end());
    User::users->erase(User::users->begin(), User::users->end());

    ConsoleLogger::Write("Deleting pointers to old-DB", LogType::Message);
    delete Device::devices;
    delete User::users;

    Device::devices = nullptr;
    User::users = nullptr;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    ConsoleLogger::Write("Updating database...", LogType::Message);
    Device::devices = MyServerApplication::ReloadDevicesFromXML();
    User::users = MyServerApplication::ReloadUsersFromXML();

//    timer.stop();

    ConsoleLogger::Write(boost::str(boost::format("Database update completed. Elapsed time: %1%") % 1/*timer.format()*/), LogType::Message);
   
}

ServerDevice::~ServerDevice() {
}
