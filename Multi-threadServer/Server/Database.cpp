#include "Database.h"



Database::Database()
{
}


Database::~Database()
{
}

void Database::loadData() {
	std::string data;
	f.open("userdata.txt", std::ios::in);

	if (f.fail())
		std::cout << "Opening file fail" << std::endl;
	else {
		int i = 1;
		while (!f.eof()) {
			getline(f, data);
			User user;
			if (data.compare("") != 0) {
				char data_temp[100];
				strcpy_s(data_temp, data.c_str());
				if (i == 1) {
					user.setUsername(data_temp);
					i++;
					continue;
				}
				else if (i == 2) {
					user.setFullName(data_temp);
					i++;
					continue;
				}
				else if (i == 3) {
					user.setGender(data_temp);
					i++;
					continue;
				}
				else if (i == 4) {
					user.setBirthday(data_temp);
					i++;
					if (i == 5) {
						usersData.push_back(user);
						i = 1;
					}
				}
			}
			else continue;
		}
		f.close();
	}
}

void Database::saveData() {
	f.open("userdata.txt", std::ios::out);
	if (f.fail())
		std::cout << "Open file fail" << std::endl;
	else {
		for (std::vector<User>::iterator i = usersData.begin(); i != usersData.end(); i++) {
			f << i->getUsername() << std::endl;
			f << i->getFullname() << std::endl;
			f << i->getGender() << std::endl;
			f << i->getDateOfBirth() << std::endl;
		}
	}
	f.close();
}

bool Database::checkUsername(std::string username) {
	for (std::vector<User>::iterator i = usersData.begin(); i != usersData.end(); ++i) {
		if (username.compare(std::string(i->getUsername())) == 0)
			return true;
	}
	return false;
}

char* Database::getUserInfor(std::string username) {
	char* result;
	char temp[1024];
	for (int i = 0; i < usersData.size(); ++i) {
		if (username.compare(usersData[i].getUsername()) == 0) {
			strcat_s(temp, "User: ");
			strcat_s(temp, usersData[i].getUsername());
			strcat_s(temp, ",");
			strcat_s(temp, "Fullname: ");
			strcat_s(temp, usersData[i].getFullname());
			strcat_s(temp, ",");
			strcat_s(temp, "Gender: ");
			strcat_s(temp, usersData[i].getGender());
			strcat_s(temp, ",");
			strcat_s(temp, "Birthday: ");
			strcat_s(temp, usersData[i].getDateOfBirth());
		}
	}
	result = temp;
	return result;
}