#pragma once
#include <Poco/Net/HTTPRequestHandlerFactory.h>

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
	MyRequestHandlerFactory();
	~MyRequestHandlerFactory();

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
};

