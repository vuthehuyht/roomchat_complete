#pragma once
#include <string>
#include <map>
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "Room.h"
#include<thread>
#include "Database.h"
class Session
{
public:

	void add(SOCKET s, std::string username);
	void addForbiddenWord(std::string word);
	void removeForbiddenWord(std::string word);
	void messageHandle(SOCKET s);
private:
	std::map<SOCKET, std::string> connections;
	Database db;
private:
	// processing message sent
	void processMessage(char message[1024], SOCKET s);
	void loadForbiddenWord();
	void saveForbiddenWord();
	bool checkForbiddenWord(std::string word);
	void removeSocket(std::string username);

private:
	std::vector<std::string> splitMessage(char message[]);
	std::vector<std::string> forbiddenWord;
};
static Session sessionptr;
