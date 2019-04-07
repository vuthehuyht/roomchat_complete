#include "Room.h"



Room::Room()
{
}


Room::~Room()
{
}

void Room::joinRoom(std::string username) {
	std::cout << "Add " << std::this_thread::get_id() << std::endl;
	std::cout << memberStatus.size() << std::endl;
	if (memberStatus.empty()) {
		memberStatus.insert(std::make_pair(username, "admin"));
		std::cout << memberStatus.size() << std::endl;
		saveStatus();
	}
	else {
		if (checkMember(username)) {
			memberStatus.insert(std::make_pair(username, "normal"));
			std::cout << memberStatus.size() << std::endl;
			saveStatus();
		}
	}
}

bool Room::checkMember(std::string username) {
	for (std::map<std::string, std::string>::iterator i = memberStatus.begin(); i != memberStatus.end(); ++i) {
		if (username.compare(i->first) == 0)
			return false;
	}
	return true;
}

void Room::disconnect(std::string username) {
	std::cout << "Remove " << memberStatus.size() << std::endl;
	std::cout << std::this_thread::get_id() << std::endl;
	std::map<std::string, std::string>::iterator i = memberStatus.find(username);
	if (i != memberStatus.end())
		memberStatus.erase(i);
	if (memberStatus.size() > 1)
		switchAdmin();
	std::cout << "Room have " << memberStatus.size() << " member" << std::endl;
}

void Room::switchAdmin() {
	std::map<std::string, std::string>::iterator i;
	i = memberStatus.begin();
	i->second.replace(i->second.begin(), i->second.end(), "admin");
	saveStatus();
}

void Room::saveStatus() {
	std::fstream f;
	f.open("roomstatus.txt", std::ios::out);
	for (std::map<std::string, std::string>::iterator i = memberStatus.begin(); i != memberStatus.end(); ++i) {
		f << i->first.data() << std::endl;
		f << i->second.data() << std::endl;
	}
	f.close();
}

void Room::loadStatus() {
	std::fstream f;
	f.open("roomstatus.txt", std::ios::in);
	if (f.fail())
		std::cout << "Opening file fai!" << std::endl;
	else {
		std::string data;
		std::string data_1;
		std::string data_2;
		int i = 1;
		while (!f.eof()) {
			getline(f, data);
			if (i == 1) {
				data_1 = data;
				i++;
				data.clear();
				continue;
			}
			if (i == 2) {
				data_2 = data;
				i++;
				if (i == 3) {
					memberStatus.insert(std::make_pair(data_1, data_2));
					data_1.clear();
					data_2.clear();
					i = 1;
				}
				data.clear();
			}
		}
	}
	f.close();
}

bool Room::checkKickUsername(std::string username) {
	for (int i = 0; i < kickOut.size(); i++) {
		if (kickOut[i].compare(username) == 0)
			return false;
	}
	return true;
}

std::string Room::getType(std::string username) {
	return memberStatus[username];
}

void Room::addKickedUsername(std::string username) {
	kickOut.push_back(username);
}

void Room::removeKickedUsername(std::string username) {
	for (int i = 0; i < kickOut.size(); i++) {
		if (kickOut[i].compare(username) == 0)
			kickOut.erase(kickOut.begin() + i);
	}
}

void Room::giveMod(std::string username) {
	std::map<std::string, std::string>::iterator i;
	i = memberStatus.find(username);
	i->second.replace(i->second.begin(), i->second.end(), "mod");
	saveStatus();
}

void Room::removeMod(std::string username) {
	std::map<std::string, std::string>::iterator i;
	i = memberStatus.find(username);
	i->second.replace(i->second.begin(), i->second.end(), "normal");
	saveStatus();
}
