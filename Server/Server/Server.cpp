#include "MyListenSocket.h"
MyListenSocket ListenSocket;

// обработка сообщений клиентов в параллельном потоке
static void ClientHandler(int index) {
	Packet packettype;

	while (true) {
		recv(ListenSocket.Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		// обработка сообщени€ от клиента
		if (!ListenSocket.ProcessPacket(index, packettype)) break;
	}

	// закрытие сокета дл€ заданного соединени€
	closesocket(ListenSocket.Connections[index]);
	ListenSocket.Connections[index] = 0;
	CloseHandle(ListenSocket.Handle[index]);
	std::cout << "ѕользователь отключилс€ от сервера!\n";
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "RU");// кириллица вывода в консоль
	
	std::cout << "—ервер готов к работе и ожидает подключени€...\n";
	// поддержка 100 соединений и их прослушка
	for (int i = 0; i < 100; i++) {
		
		// проверка успешности подключени€ к серверу нового сокета
		if (ListenSocket.CreateConnection(i) == 0) {
			std::cout << "Ѕезуспешное подключение пользовател€ к серверу\n";
		}
		else {
			std::cout << "Ќовый пользователь подключилс€ к чату!\n";
			//ListenSocket.SendMessageToCLient();
			// создание дополнительного потока дл€ клиента
			ListenSocket.Handle[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

			Packet testpacket = P_Test;
			send(ListenSocket.newConnection, (char*)&testpacket, sizeof(Packet), NULL);
		}
	}


	system("pause");
	return 0;
}