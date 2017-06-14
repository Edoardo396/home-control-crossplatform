#include "../HomeControlController (Windows)/stdafx.h"
#include "MyRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include <Poco/Format.h>
#include "ConsoleLogger.h"
#include <Poco/Net/HTTPServerResponse.h>
#include <ostream>
#include <Poco/StringTokenizer.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "User.h"
#include <boost/format.hpp>
#include "Device.h"
#include <algorithm>

MyRequestHandler::~MyRequestHandler() {
}

void MyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

	using namespace std;

	response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	response.setContentType("text/html");

	std::ostream& out = response.send();

	Dictionary params = ParseUri(request.getURI());

	User invoker = User::Login(params["username"], params["password"]);

	ConsoleLogger::Write((boost::format("%1% requested %2% on %3% from %4%") 
		% invoker.getName() 
		% params["request"]
		% params["device"] 
		% request.clientAddress().toString()).str(), LogType::Message);

	// TODO Check for const!
	auto match = std::find_if(Device::devices->begin(), Device::devices->end(), [&params](const Device* dev) {return dev->getName() == params["device"]; });
	
	if(match == Device::devices->end()) {
		out << "Target not found";
		return;
	}

	Device* target = *match;

    out << target->ParseCommand(params["request"], params, invoker);
	
	if(invoker.getId() == User::ANONYMOUS.getId()) {
		out << "Unautorized";
		return;
	}
}

Dictionary MyRequestHandler::ParseUri(std::string uri) {

	if (uri[0] == '/')
		uri.erase(0, 2);

	auto map = std::map<std::string, std::string>();

	std::vector<std::string> main;

	boost::split(main, uri, boost::is_any_of("&="), boost::token_compress_on);

	if (main.size() < 2 || main.size() % 2 != 0)
		return map;

	for (int i = 0; i < main.size() - 1; i += 2)
		map.insert(std::pair<std::string, std::string>(main[i], main[i + 1]));

	return map;
}
