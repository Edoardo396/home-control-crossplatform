#pragma once
#include <list>
#include <ostream>

class User {

	int id;
	std::string name;
	std::string password;
	unsigned int accessLevel;

public:

	static User ANONYMOUS;

	int getId() const { return id; }
	std::string getName() const { return name; }
	std::string getPassword() const { return password; }
	unsigned getAccessLevel() const { return accessLevel; }

	User(int id, const std::string& name, const std::string& password, unsigned access_level)
		: id(id),
		  name(name),
		  password(password),
		  accessLevel(access_level) {
	}

	static User Login(std::string username, std::string password);
    std::vector<const class Device*> GetMyDevices() const;
    static std::list<User>* users;

	friend std::ostream& operator<<(std::ostream& os, const User& obj) {
		return os
			<< "id: " << obj.id
			<< "name: " << obj.name.c_str()
			<< "password: " << obj.password.c_str()
			<< "accessLevel: " << obj.accessLevel;
	}

	std::string ToString() const;
};