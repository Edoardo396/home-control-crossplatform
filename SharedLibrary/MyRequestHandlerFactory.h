#pragma once
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <list>

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
	MyRequestHandlerFactory() {};
	~MyRequestHandlerFactory();

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
	std::list<class Device>* devices;
};

