#include "../SharedLibrary/stdafx.h"
#include "MyServerApplication.h"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPResponse.h>
#include <fstream>
#include <Poco/Net/HTTPServerResponse.h>
#include "MyRequestHandlerFactory.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "User.h"
#include <boost/thread.hpp>
#include "ConsoleLogger.h"
#include "ArduinoUnlocker.h"
#include "Macros.h"
#include <boost/format.hpp>
#include "ServerDevice.h"
#include "DaikinAC.h"
#include <string>
#include "ArduinoTemperature.h"
#include "Command.h"


using std::cout;
using std::endl;


std::string MyServerApplication::filePath;


MyServerApplication::~MyServerApplication() {

	Device::devices->erase(Device::devices->begin(), Device::devices->end());
	delete Device::devices;

	User::users->erase(User::users->begin(), User::users->end());
	delete User::users;
}

std::list<Device*>* MyServerApplication::ReloadDevicesFromXML() {

    ConsoleLogger::Write("Reloading devices list...", LogType::Message);

	using namespace Poco::XML;


	auto devlist = new std::list<Device*>();

	std::ifstream in(filePath);
	InputSource src(in);

	DOMParser parser;
	AutoPtr<Document> pDoc = parser.parse(&src);

	auto devicenode = pDoc->getElementsByTagName("Device");

	for (int i = 0; i < devicenode->length(); ++i) {
		auto device = devicenode->item(i);

#pragma region LOAD_DEV_BY_TYPE
        
		DVLOAD devtmp = { DEVXMLTEXTOF(/Type), DEVXMLTEXTOF(/DisplayName), DEVXMLTEXTOF(/Nome), std::stoi(DEVXMLTEXTOF(/RAL)), std::stoi(DEVXMLTEXTOF(/Port)), Poco::Net::IPAddress::parse(DEVXMLTEXTOF(/IPAddress)) };

        if (devtmp.type == "ArduinoUnlocker")
            devlist->push_back(new ArduinoUnlocker(devtmp.name, devtmp.ipaddr, devtmp.ral, devtmp.port, devtmp.dispname, Device::State::Unknown));
        else if (devtmp.type == "DaikinAC")
            devlist->push_back(new DaikinAC(devtmp.name, devtmp.ipaddr, devtmp.ral, devtmp.port, devtmp.dispname, Device::State::Unknown));
        else if (devtmp.type == "ArduinoTemperature")
            devlist->push_back(new ArduinoTemperature(devtmp.name, devtmp.ipaddr, devtmp.ral, devtmp.port, DEVXMLTEXTOF(/DisplayName), Device::State::Unknown, Device::LocationByText(DEVXMLTEXTOF(/KTLocation)), DEVXMLTEXTOF(/AutoStart) == "true", std::stof(DEVXMLTEXTOF(/MyTemp))));
		else
			ConsoleLogger::Write((boost::format("Not recognized type \"%1%\" of %2%") % devtmp.type % devtmp.name).str(), LogType::Warning);

#pragma endregion 

	
	}

    auto serverSelfCommands = [](std::string request, Dictionary params, class User invoker, ServerDevice* sender) -> std::string
	{
        if (request == "ping")
            return "Pong!";

        if (request == "login")
            return invoker.getId() != -1 ? std::string("true;") + std::to_string(invoker.getAccessLevel()) : "false";

        if (request == "getMyDevices")
            return Device::SerializeListDevDisp(invoker.GetMyDevices());

        if (request == "srv-shutdown" && invoker.getAccessLevel() >= 80) {
            sender->SetOff();
            return "true";
        }

        if (request == "update-db" && invoker.getAccessLevel() >= 60) {
            sender->UpdateDB();
            return "true";
        }

        return "CommandNotFound";
	};


	devlist->push_back(new ServerDevice("ServerSelf", Poco::Net::IPAddress(std::string("::1"), Poco::Net::IPAddress::IPv6), 0, 8080, "Server", Device::State::Reachable, serverSelfCommands));

	return devlist;
}

// TODO rewrite better
std::list<User>* MyServerApplication::ReloadUsersFromXML() {

	using namespace Poco::XML;

	auto usersList = new std::list<User>();

	std::ifstream in(filePath);
	InputSource src(in);

	DOMParser parser;
	AutoPtr<Document> pDoc = parser.parse(&src);

	// TODO check for duplicated
	auto users = pDoc->getElementsByTagName("User");

	for (int i = 0; i < users->length(); ++i) {

		// get user node
		auto user = users->item(i);

		usersList->push_back(User(
			stoi(user->attributes()->item(0)->innerText()),
			user->getNodeByPath("//Nome")->innerText(),
			user->getNodeByPath("//Password")->innerText(),
			stoi(user->getNodeByPath("//AL")->innerText())));
	}

	return usersList;
}

int MyServerApplication::main(const std::vector<std::string>&) {

	using namespace Poco::Net;

	ConsoleLogger::Write("Creating users list...", LogType::Message);

	User::users = ReloadUsersFromXML();
	Device::devices = ReloadDevicesFromXML();

	ConsoleLogger::Write("Starting HTTPServer On Port " + std::to_string(port), LogType::Message);

	HTTPServer server(new MyRequestHandlerFactory(), ServerSocket(port), new HTTPServerParams);

	server.start();

	ConsoleLogger::Write("Server Started, Waiting for Commands...", LogType::Message);

	waitForTerminationRequest();

	std::cout << "Shutting down... " << std::endl;
	server.stop();

	return Application::EXIT_OK;
}
