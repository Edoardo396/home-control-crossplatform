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


MyRequestHandler::MyRequestHandler() {
}


MyRequestHandler::~MyRequestHandler() {
}

void MyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	
	using namespace std;

	response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	response.setContentType("text/html");
		
	std::ostream& out = response.send();
	
	map<string,string> params = ParseUri(request.getURI());




}

Dictionary MyRequestHandler::ParseUri(std::string uri) {

	if (uri[0] == '?')
		uri.erase(0, 1);

	auto map = std::map<std::string, std::string>();

	std::vector<std::string> main;

	boost::split(main, uri, boost::is_any_of("&="), boost::token_compress_on);

	for(int i = 0;i<main.size() - 2; i += 2) 	
		map.insert(std::pair<std::string, std::string>(main[i], main[i + 1]));


	return map;
}
