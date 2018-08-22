#include "Session.h"

void Session::add(SOCKET s, std::string username) {
	connections.insert(std::make_pair(s, username));
}

void Session::messageHandle(SOCKET s) {
	char buffer[1024];
	std::string username = connections[s];
	while (true) {
		ZeroMemory(buffer, sizeof(buffer));
		if (recv(s, buffer, sizeof(buffer), 0) > 0) {
			char buffer_temp_1[1024];
			strcpy_s(buffer_temp_1, buffer);
			char buffer_temp_2[1024];
			strcpy_s(buffer_temp_2, buffer);

			if (strcmp(buffer_temp_1, "pp") != 0) {
				std::cout << "User " << username << ": " << buffer << std::endl;
				if (room.checkKickUsername(username))
					processMessage(buffer_temp_2, s);
				else {
					char message[] = "You can't send this message";
					send(s, message, sizeof(message), 0);
				}
			}
			else {
				room.disconnect(username);
				removeSocket(username);
			}
		}
	}
}

void Session::processMessage(char message[1024], SOCKET s) {
	std::vector<std::string> splitedMessage = splitMessage(message);
	if (splitedMessage.size() != 0) {
		std::vector<std::string>::iterator first = splitedMessage.begin();
		std::vector<std::string>::iterator second = splitedMessage.begin() + 1;

		std::string username = connections[s];
		if (room.getType(username).compare("admin") == 0) {
			if (first->compare("/ban") == 0)
				room.addKickedUsername(second->data());
			else if (first->compare("/unban") == 0)
				room.removeKickedUsername(second->data());
			else if (first->compare("/info") == 0) {
				char data[1024];
				strcpy_s(data, db.getUserInfor(username));
				strcat_s(data, ",Type: ");
				strcat_s(data, room.getType(username).c_str());
				puts(data);
				send(s, data, sizeof(data), 0);
			}
			else if (first->compare("/mod") == 0)
				room.giveMod(second->data());
			else if (first->compare("/unmod") == 0)
				room.removeMod(username);
		}
		else if (room.getType(username).compare("admin") == 0 || room.getType(username).compare("mod") == 0) {
			if (first->compare("/filter") == 0) {
				addForbiddenWord(second->data());
				saveForbiddenWord();
			}
			else if (first->compare("/unfilter") == 0) {
				removeForbiddenWord(second->data());
				saveForbiddenWord();
			}
		}
		else {
			std::map<SOCKET, std::string>::iterator it;
			std::string username = connections[s];
			char completelyMesseage[1024];
			ZeroMemory(completelyMesseage, sizeof(completelyMesseage));
			for (int i = 0; i < splitedMessage.size(); i++) {
				if (checkForbiddenWord(splitedMessage[i])) {
					splitedMessage[i].replace(splitedMessage[i].begin(), splitedMessage[i].end(), "***");
					strcat_s(completelyMesseage, splitedMessage[i].c_str());
					strcat_s(completelyMesseage, " ");
				}
				else {
					strcat_s(completelyMesseage, splitedMessage[i].c_str());
					strcat_s(completelyMesseage, " ");
				}
			}
			for (it = connections.begin(); it != connections.end(); ++it) {
				char buffer_temp[1024];
				ZeroMemory(buffer_temp, sizeof(buffer_temp));
				if (username.compare(it->second) != 0 && room.checkKickUsername(it->second)) {
					strcat_s(buffer_temp, username.c_str());
					strcat_s(buffer_temp, ": ");
					strcat_s(buffer_temp, completelyMesseage);
					send(it->first, buffer_temp, sizeof(buffer_temp), 0); // gửi tới các user khác user truyền vào
					std::cout << "Send user " << it->second << ": " << completelyMesseage << std::endl;
				}
			}
		}
	}
}

std::vector<std::string> Session::splitMessage(char message[]) {
	char* p;
	char* next_token = NULL;
	std::vector<std::string> result;

	p = strtok_s(message, " ", &next_token);
	while (p != NULL) {
		result.push_back(std::string(p));
		p = strtok_s(NULL, " ", &next_token);
	}
	return result;
}

void Session::addForbiddenWord(std::string word) {
	forbiddenWord.push_back(word);
}

void Session::removeForbiddenWord(std::string word) {
	for (std::vector<std::string>::iterator i = forbiddenWord.begin(); i != forbiddenWord.end(); i++) {
		if (word.compare(i->data()) == 0)
			forbiddenWord.erase(i);
	}
}

void Session::loadForbiddenWord() {
	std::fstream f;
	f.open("forbiddenword.txt", std::ios::in);
	if (f.fail())
		std::cout << "Opening file fail" << std::endl;
	else {
		std::string data;
		while (f.eof()) {
			getline(f, data);
			forbiddenWord.push_back(data);
		}
	}
	f.close();
}

void Session::saveForbiddenWord() {
	std::fstream f;
	f.open("forbiddenword.txt", std::ios::out);
	for (int i = 0; i < forbiddenWord.size(); i++)
		f << forbiddenWord[i] << std::endl;
	f.close();
}

bool Session::checkForbiddenWord(std::string word) {
	for (int i = 0; i < forbiddenWord.size(); i++) {
		if (word.compare(forbiddenWord[i]) == 0)
			return true;
	}
	return false;
}

void Session::removeSocket(std::string username) {
	for (std::map<SOCKET, std::string>::iterator i = connections.begin(); i != connections.end(); i++) {
		if (username.compare(i->second.data()) == 0)
			connections.erase(i);
	}
}