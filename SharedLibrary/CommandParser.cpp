#include "../HomeControlController (Windows)/stdafx.h"
#include "CommandParser.h"
#include "User.h"
#include <boost/format.hpp>

std::string CommandParser::ExecuteServerSelfCommand(Dictionary request, User invoker) {

	if (request["request"] == "login")
		return invoker.getId() != -1 ? str(boost::format("%1%;%2%") % "true" % invoker.getAccessLevel()) : "false";

	return "CommandNotFound";
}

std::string CommandParser::ExecuteCommand(Dictionary request) {

	std::transform(request["device"].begin(), request["device"].end(), request["device"].begin(), ::tolower);

	User invoker = User::Login(request["username"], request["password"]);


	if (request["device"] == "serverself")
		return ExecuteServerSelfCommand(request, invoker);

	return "false";
}

CommandParser::~CommandParser() {
}
