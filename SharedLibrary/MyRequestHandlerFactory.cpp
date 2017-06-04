#include "../HomeControlController (Windows)/stdafx.h"
#include "MyRequestHandlerFactory.h"
#include "MyRequestHandler.h"


MyRequestHandlerFactory::MyRequestHandlerFactory() {


}


MyRequestHandlerFactory::~MyRequestHandlerFactory() {
}

Poco::Net::HTTPRequestHandler* MyRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
	return new MyRequestHandler();
}
