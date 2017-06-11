#pragma once
#include "Device.h"
#include "Macros.h"
class User;

class CommandParser {

	std::vector<Device>* devices;
	

	std::string ExecuteServerSelfCommand(Dictionary request, User invoker);

public:
	std::string ExecuteCommand(Dictionary request);

	explicit CommandParser(std::vector<Device>* dev_list_ptr) : devices(dev_list_ptr) {}
	~CommandParser();

};
