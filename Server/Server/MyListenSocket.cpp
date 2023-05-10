#include "MyListenSocket.h"

MyListenSocket::MyListenSocket() {
	this->Counter = 0;
	// создание библиотеки
	this->createVersion();
	// создание адреса
	this->createAddress();


	// сокет с результатом выполнения
	// использование семейства протоколов с указанным протоколом для соединения (SOCK_STREAM)
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	// привязка адреса с сокетом
	bind(sListen, (SOCKADDR*)&addrSocket, sizeof(addrSocket));
	// прослушивание порта в ожидании соединения со стороны клиента
	listen(sListen, SOMAXCONN);
}


// создание библиотеки
void MyListenSocket::createVersion() {
	// запрос версии библиотеки winsock для загрузки библиотеки
	WORD DLLVersion = MAKEWORD(2, 1);
	// загрузка библиотеки заданной версии

	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Ошибка загрузки библиотеки заданной версии!" << std::endl;
		exit(1);
	}
}


// создание адреса
void MyListenSocket::createAddress()
{
	// заполнение информации об адресе сокета
	addrSocket.sin_addr.s_addr = inet_addr("127.0.0.1");// хранит IP адрес
	addrSocket.sin_port = htons(1111);// порт для идентификации программы с поступающими данными
	addrSocket.sin_family = AF_INET;// семейство протоколов
	sizeofaddr = sizeof(addrSocket);
}


// создание подключения к серверу нового сокета
bool MyListenSocket::CreateConnection(int indexConnection)
{
	// addr - содержит данные адреса клиента
	newConnection = accept(sListen, (SOCKADDR*)&addrSocket, &sizeofaddr);
	// проверка успешности подключения к серверу
	// безуспешное подключение клиента к серверу
	if (newConnection == 0) return 0;

	// добавление соединения в массив соединений
	Connections[indexConnection] = newConnection;
	Counter++;
	return 1;
}

// пакет обработки сообщения от клиента и рассылка сообщения всем клиентам
bool MyListenSocket::ProcessPacket(int index, Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		if (msg_size < 0) return false;
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		// получение сообщение от клиента
		recv(Connections[index], msg, msg_size, NULL);

		for (int i = 0; i < Counter; i++) {
			Packet msgtype = P_ChatMessage;// тип передачи данных
			// первый параметр хранит соединения с клиентом, которому отправляем сообщение
			// передача типа данных и типа и подготовка размера сообщения
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			// передача сообщения
			send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		break;
	}
	default:
		std::cout << "Нераспознанный пакет: " << packettype << std::endl;
		return false;
		break;
	}

	return true;
}
