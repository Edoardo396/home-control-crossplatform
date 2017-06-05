#pragma once
#include <string>
#include <map>
#include <vector>
#include <Poco/Net/IPAddress.h>




class Command {

public:
	static std::string GetCommandDir(std::map< std::string, std::string> values);

	static std::string GetCommandDir(std::string request);

	static std::string ExecuteGETRequest(Poco::Net::IPAddress IP, int port, std::string dir);

	static std::string ExecutePOSTRequest(Poco::Net::IPAddress IP, int port, std::string dir, std::map< std::string, std::string> values);
};

