#include "../HomeControlController (Windows)/stdafx.h"
#include "Command.h"
#include <string>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

std::string Command::GetCommandDir(std::map<std::string, std::string> values) {

	std::string s = "?";

	for(auto value : values) {
		s += value.first + "=";
		s += value.second + "&";
	}

	s.erase(s.end() - 1);
	return s;
}

std::string Command::ExecuteGETRequest(Poco::Net::IPAddress IP, int port, std::string dir) {

	using namespace Poco::Net;

	std::string ip = IP.toString();

	auto clientSession = new HTTPClientSession(ip, Poco::UInt16(port));

	auto request = new HTTPRequest(HTTPRequest::HTTP_GET, dir, HTTPMessage::HTTP_1_1);

	clientSession->sendRequest(*request);

	HTTPResponse response;

	std::istream& is = clientSession->receiveResponse(response);

	auto sresponse = std::string(std::istreambuf_iterator<char>(is), {});

	delete clientSession;
	delete request;

	return sresponse;
}

std::string Command::ExecutePOSTRequest(Poco::Net::IPAddress IP, int port, std::string dir, std::map<std::string, std::string> values) {


	using namespace Poco::Net;

	std::string ip = IP.toString();

	auto clientSession = new HTTPClientSession(ip, Poco::UInt16(port));

	auto request = new HTTPRequest(HTTPRequest::HTTP_POST, dir, HTTPMessage::HTTP_1_1);

	for (auto pair : values) request->add(pair.first, pair.second);

	clientSession->sendRequest(*request);

	HTTPResponse response;

	std::istream& is = clientSession->receiveResponse(response);

	auto sresponse = std::string(std::istreambuf_iterator<char>(is), {});

	delete clientSession;
	delete request;

	return sresponse;
}
