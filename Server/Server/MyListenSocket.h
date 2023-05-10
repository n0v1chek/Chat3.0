#pragma once
#pragma comment(lib, "ws2_32.lib")// для доступа к некоторым функциям
#include <winsock2.h>
#include <iostream>
// устранение предупреждений
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

enum Packet {
	P_ChatMessage,
	P_Test
};




class MyListenSocket
{
private:
	WSAData wsaData;// информация о работе и версии библиотеки winsock

public:
	SOCKET Connections[100];// массив соединений
	int Counter;// количество соединений с сервером
	HANDLE Handle[100];// идентификатор потока


	// заполнение информации об адресе сокета
	SOCKADDR_IN addrSocket;
	int sizeofaddr;

	// сокет с результатом выполнения
	// использование семейства протоколов с указанным протоколом для соединения (SOCK_STREAM)
	SOCKET sListen;
	// новое подключение
	SOCKET newConnection;



	// конструктор класса
	MyListenSocket();

	// создание библиотеки
	void createVersion();

	// создание адреса
	void createAddress();

	// создание подключения к серверу нового сокета
	bool CreateConnection(int indexConnection);

	// пакет обработки сообщения от клиента и рассылка сообщения всем клиентам
	bool ProcessPacket(int index, Packet packettype);

};
