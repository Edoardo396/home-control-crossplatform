#include "../HomeControlController (Windows)/stdafx.h"
#include "MyRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include <Poco/Format.h>
#include "../Support/ConsoleLogger.h"
#include <Poco/Net/HTTPServerResponse.h>
#include <ostream>


MyRequestHandler::MyRequestHandler() {
}


MyRequestHandler::~MyRequestHandler() {
}

void MyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) { 
	response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
	response.setContentType("text/html");
	std::ostream& out = response.send();
	out << "<h1>Hello world!</h1>" 
		<< "<p>Host: " << request.getHost() << "</p>"
		<< "<p>Method: " << request.getMethod() << "</p>"
		<< "<p>URI: " << request.getURI() << "</p>";
	out.flush();


	std::cout << std::endl
		<< " and URI=" << request.getURI() << std::endl;

}
