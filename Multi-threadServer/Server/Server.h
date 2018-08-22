#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <thread>
#include "Database.h"
#include "Room.h"
#include "Session.h"
class Server
{
public:
	Server(int port);
	bool listenForNewConnection();

private:

	static void clientHandle(SOCKET s, std::string username);
private:
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	SOCKET listening;
	Database db;
	Room room;
	char msgOK[3] = "OK";
};
static Server* serverptr;

