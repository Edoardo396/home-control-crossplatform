#include "../HomeControlController (Windows)/stdafx.h"
#include "User.h"
#include <string>
#include <algorithm>
#include "Device.h"

// Static Members

std::list<User>* User::users;
User User::ANONYMOUS = User(-1, "", "", 0);


User User::Login(std::string username, std::string password) {

	auto user = std::find_if(User::users->begin(), User::users->end(), [username, password](const User& u) {return u.getName() == username && u.getPassword() == password; });

	return user != users->end() ? *user : User(-1, "Anonymous", "", 0);
}

std::vector<const Device*> User::GetMyDevices() const {
    auto rtn = std::vector<const Device*>();

    for(auto it = Device::devices->begin(); it != Device::devices->end(); ++it) {
        Device* dev = *it;
        if (dev->getRequiredAccessLevel() <= accessLevel && dev->getState() != Device::State::NotReachable && dev->getState() != Device::State::Unknown && dev->getName() != "ServerSelf")
            rtn.push_back(dev);
    }

    return rtn;
}

std::string User::ToString() const {
	return "id: " + std::to_string(id) + "name: " + name + "password: " + password + "accesslevel: " + std::to_string(accessLevel);
}
