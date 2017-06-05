#include "../HomeControlController (Windows)/stdafx.h"
#include "User.h"
#include <string>

User User::Login(std::string username, std::string password) {



	return User(0, "", "", 0);
}

std::string User::ToString() const {
	return "id: " + std::to_string(id) + "name: " + name + "password: " + password + "accesslevel: " + std::to_string(accessLevel);
}
