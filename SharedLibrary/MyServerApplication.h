#pragma once

#include <Poco/Util/ServerApplication.h>
#include "Support/User.h"


class MyServerApplication : public Poco::Util::ServerApplication {
public:
	MyServerApplication();
	~MyServerApplication();
protected:
	int main(const std::vector<std::string> &) override;

	auto ReloadUsersFromXML();
};

