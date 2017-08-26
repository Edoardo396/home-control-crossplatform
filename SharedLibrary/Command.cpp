#include "../SharedLibrary/stdafx.h"
#include "Command.h"
#include <string>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include "Macros.h"
#include "ConsoleLogger.h"

std::string Command::GetCommandDir(std::map<std::string, std::string> values) {

	std::string s = "?";

	for(auto value : values) {
		s += value.first + "=";
		s += value.second + "&";
	}

	s.erase(s.end() - 1);
	return s;
}

std::string Command::GetCommandDir(std::string request) {
	Dictionary map = { { "request", request } };
	return GetCommandDir(map);
}

std::string Command::ExecuteGETRequest(Poco::Net::IPAddress IP, int port, std::string dir, int timeout, bool direct) {

	using namespace Poco::Net;

    if (!direct)
        dir = "?request=" + dir;

	std::string ip = IP.toString();

	auto clientSession = new HTTPClientSession(ip, Poco::UInt16(port));

    clientSession->setTimeout(Poco::Timespan(timeout, 0L));

	auto request = new HTTPRequest(HTTPRequest::HTTP_GET, dir, HTTPMessage::HTTP_1_1);

	clientSession->sendRequest(*request);

	HTTPResponse response;

	std::istream& is = clientSession->receiveResponse(response);

	auto sresponse = std::string(std::istreambuf_iterator<char>(is), {});

	delete clientSession;
	delete request;

	return sresponse;
}

std::string Command::ExecutePOSTRequest(Poco::Net::IPAddress IP, int port, std::string dir, std::vector<std::pair<std::string, std::string>> values) {


	using namespace Poco::Net;

	std::string ip = IP.toString();

	auto clientSession = new HTTPClientSession(ip, Poco::UInt16(port));


	auto request = new HTTPRequest(HTTPRequest::HTTP_POST, dir, HTTPMessage::HTTP_1_1);

    request->setContentType("raw");

	std::string requeststr = "";

	for (auto it = values.begin(); it != values.end(); ++it) {
		requeststr += (it->first + "=" + it->second + "&");
	}

	requeststr.erase(requeststr.size() - 1, 1);

	request->setContentLength(requeststr.length());

    std::ostream& osstream = clientSession->sendRequest(*request);
    osstream << requeststr;


	HTTPResponse response;

	std::istream& is = clientSession->receiveResponse(response);

	auto sresponse = std::string(std::istreambuf_iterator<char>(is), {});
    
#ifdef _DEBUG
    ConsoleLogger::Write(sresponse, LogType::Message);
#endif

//    ConsoleLogger::Write(IP.toString() + " response: " + sresponse, LogType::Message);

	delete clientSession;
	delete request;

	return sresponse;
}
