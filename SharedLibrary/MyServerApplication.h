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

    friend class ServerDevice;

private:
    static const std::string filePath;

public:
	MyServerApplication();
	~MyServerApplication();
protected:
	int main(const std::vector<std::string> &) override;

	static std::list<Device*>* ReloadDevicesFromXML();
	static std::list<User>* ReloadUsersFromXML();
};

