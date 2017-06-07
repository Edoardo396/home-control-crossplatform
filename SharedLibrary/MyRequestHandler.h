#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include "Macros.h"

class MyRequestHandler : public Poco::Net::HTTPRequestHandler {

public:
	MyRequestHandler();
	~MyRequestHandler();
	
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
private:
	Dictionary ParseUri(std::string uri);

};

