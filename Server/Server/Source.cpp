#include "Server.h"

int main() {
	Server server(1111);
	for (int i = 0; i < 100; i++) {
		server.listenForNewConnection();
	}
	system("pause");
	return 0;

	/*roomptr.joinRoom("abc");
	roomptr.joinRoom("demo");
	roomptr.disconnect("demo");
	system("pause");*/
}