#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
class Room
{
public:
	Room();
	~Room();

	void joinRoom(std::string username);
	void loadStatus();
	bool checkKickUsername(std::string username);
	std::string getType(std::string username);
	void addKickedUsername(std::string username);
	void removeKickedUsername(std::string username);
	void giveMod(std::string username);
	void removeMod(std::string username);
	void disconnect(std::string username);
	
private:
	std::map<std::string, std::string> memberStatus;
	std::vector<std::string> kickOut;

private:
	bool checkMember(std::string username);
	void switchAdmin();
	void saveStatus();
};

