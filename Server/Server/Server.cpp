#include "Server.h"

Server::Server(int port) {
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(port); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	listening = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	if (bind(listening, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(listening, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	db.loadData();
	roomptr.loadStatus();
}

bool Server::listenForNewConnection() {
	std::cout << "Waiting..." << std::endl;
	SOCKET newConnection = accept(listening, (SOCKADDR*)&addr, &addrlen);
	if (newConnection == 0) {
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else {
		char data[1024];
		ZeroMemory(data, sizeof(data));
		recv(newConnection, data, sizeof(data), 0);
		if (strcmp(data, "1") == 0) {
			send(newConnection, msgOK, 3, 0);
			ZeroMemory(data, sizeof(data));
			recv(newConnection, data, sizeof(data), 0);
			if (db.checkUsername(std::string(data))) {
				std::cout << "Client connnected!" << std::endl;
				char msg[30] = "Connnection successfully";
				send(newConnection, msg, sizeof(msg), 0);
				std::thread t(clientHandle, newConnection, std::string(data));
				t.detach();
			}
		}
	}
	return true;
}

void Server::clientHandle(SOCKET s, std::string username) {
	roomptr.joinRoom(std::string(username));
	sessionptr.add(s, username);
	sessionptr.messageHandle(s);
}