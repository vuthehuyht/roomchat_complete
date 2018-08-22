#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include <WS2tcpip.h>
#include "User.h"
#include <string>
#include <vector>

SOCKET Connection;//This client's connection to the server

std::vector<std::string> splitMessage(char message[1024]) {
	char* p;
	char* next_token;
	std::vector<std::string> result;

	p = strtok_s(message, ".,", &next_token);
	while (p != NULL) {
		result.push_back(std::string(p));
		p = strtok_s(NULL, ".,", &next_token);
	}

	return result;
}

void ClientThread()
{
	char buffer[1024]; //Create buffer to hold messages up to 256 characters
	ZeroMemory(buffer, sizeof(buffer));
	while (true){
		recv(Connection, buffer, sizeof(buffer), 0); //receive buffer
		if (strcmp(buffer, "get_info") == 0) {
			ZeroMemory(buffer, sizeof(buffer));
			strcpy_s(buffer, "OK");
			send(Connection, buffer, sizeof(buffer), 0); 
			ZeroMemory(buffer, sizeof(buffer));
			recv(Connection, buffer, sizeof(buffer), 0);

			char buffer_temp[1024];
			strcpy_s(buffer_temp, buffer);
			std::vector<std::string> result = splitMessage(buffer_temp);
			for (int i = 0; i < result.size(); i++)
				std::cout << result[i] << std::endl;

		} else 
		std::cout << buffer << std::endl; //print out buffer
	}
	ExitThread(0);
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0){
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	std::string ip = "127.0.0.1";
	sockaddr_in addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, 0); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}
	
	User user;
	int choose;
	std::cout << "If you don't have account, enter 0 to register.\nElse if you have account, enter 1 to log in" << std::endl;
	std::cout << "Your choice: ";
	std::cin >> choose;
	if (choose == 0) {					// đăng kí username
		char temp[2048];
		strcpy_s(temp, "0");
		send(Connection, temp, sizeof(temp), 0);			//gửi thông điệp lựa chọn
		if (recv(Connection, temp, sizeof(temp), 0) > 0) {
			if (strcmp(temp, "OK") == 0) {					// xác nhận đã sẵn sàng thành công
				user.create();
				ZeroMemory(temp, sizeof(temp));
				strcat_s(temp, user.getUsername());
				strcat_s(temp, ",");
				strcat_s(temp, user.getFullname());
				strcat_s(temp, ",");
				strcat_s(temp, user.getGender());
				strcat_s(temp, ",");
				strcat_s(temp, user.getDateOfBirth());
				puts(temp);
				send(Connection, temp, sizeof(temp), 0); // gửi đi chuỗi thông tin về username
			}
		}
	}
	else if (choose == 1) {									//lựa chọn đăng nhập
		char temp[1024];
		strcpy_s(temp, "1");
		send(Connection, temp, sizeof(temp), 0);			//gửi đi thông điệp đăng nhập
		ZeroMemory(temp, sizeof(temp));
		recv(Connection, temp, sizeof(temp), 0);			//nhận về thông điệp sẵn sàng
		if (strcmp(temp, "OK") == 0) {
			user.writeUser();
			ZeroMemory(temp, sizeof(temp));
			strcpy_s(temp, user.getUsername());
			send(Connection, temp, sizeof(temp), 0);		// gửi đi tên username
			if (recv(Connection, temp, sizeof(temp), 0) > 0) {
				std::cout << temp << std::endl;
				if (strcmp(temp, "Connnection successfully") == 0) {
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
					while (true) {
						char buffer[1024]; //256 char buffer to send message
						ZeroMemory(buffer, sizeof(buffer));
						std::cin.getline(buffer, sizeof(buffer));
						if (strcmp(buffer, "pp") != 0 ) {
							if (strcmp(buffer, "") != 0)
								send(Connection, buffer, sizeof(buffer), 0); //Send buffer
							else continue;
						}
						else {
							std::cout << "Exitting...." << std::endl;
							char msg[10] = "pp";
							send(Connection, msg, sizeof(msg), 0);
							break;
							exit(1);
						}
					}
				}
				else {
					recv(Connection, temp, sizeof(temp), 0);
					std::cout << temp << std::endl;
				}
			}
			else {
				recv(Connection, temp, sizeof(temp), 0);
				std::cout << temp << std::endl;
			}
		}
	}
	system("pause");
	return 0;
}