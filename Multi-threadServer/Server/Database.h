#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "User.h"
class Database
{
public:
	Database();
	~Database();

	void loadData();
	void saveData();
	bool checkUsername(std::string username);
	char* getUserInfor(std::string username);
private:
	std::fstream f;
	std::vector<User> usersData;
};

