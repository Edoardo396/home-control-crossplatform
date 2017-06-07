#include "../HomeControlController (Windows)/stdafx.h"

#include "MyServerApplication.h"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
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
#include "ConsoleLogger.h"

using std::cout;
using std::endl;

MyServerApplication::MyServerApplication() {
}


MyServerApplication::~MyServerApplication() {
}

// TODO rewrite better
auto MyServerApplication::ReloadUsersFromXML() {

	using namespace Poco::XML;

	auto usersList = std::list<User>();

	std::ifstream in("C:\\Users\\edoardo.fullin\\Desktop\\Database\\HomeControlDB.xml");
	InputSource src(in);

	DOMParser parser;
	AutoPtr<Document> pDoc = parser.parse(&src);

	// TODO check for duplicated
	auto users = pDoc->getElementsByTagName("User");

	for (int i = 0; i < users->length(); ++i) {

		// get user node
		auto user = users->item(i);

		usersList.push_back(User(
			stoi(user->attributes()->item(0)->innerText()),
			user->getNodeByPath("//Nome")->innerText(),
			user->getNodeByPath("//Password")->innerText(),
			stoi(user->getNodeByPath("//AL")->innerText())));
	}

	return usersList;
}

int MyServerApplication::main(const std::vector<std::string>&) {

	using namespace Poco::Net;

	//User::users = std::list<User>();

	ConsoleLogger::Write("Creating users list...", LogType::Message);

	ReloadUsersFromXML();

	ConsoleLogger::Write("Starting HTTPServer On Port " + std::to_string(port), LogType::Message);

	HTTPServer server(new MyRequestHandlerFactory(), ServerSocket(port), new HTTPServerParams);

	server.start();

	ConsoleLogger::Write("Server Started, Waiting for Commands...", LogType::Message);

	waitForTerminationRequest();

	std::cout << "Shutting down... " << std::endl;
	server.stop();

	return Application::EXIT_OK;
}
