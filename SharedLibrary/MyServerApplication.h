#pragma once

#include <Poco/Util/ServerApplication.h>
#include "User.h"
#include <Poco/Net/IPAddress.h>


class MyServerApplication : public Poco::Util::ServerApplication {


	struct DVLOAD {
		std::string type, dispname, name;
		int ral, port;
		Poco::Net::IPAddress ipaddr;
	};


private:
	const std::string filePath = "C:\\Users\\edoardo.fullin\\Desktop\\Database\\HomeControlDB.xml";

public:
	MyServerApplication();
	~MyServerApplication();
protected:
	int main(const std::vector<std::string> &) override;

	auto ReloadDevicesFromXML();
	auto ReloadUsersFromXML();
};

