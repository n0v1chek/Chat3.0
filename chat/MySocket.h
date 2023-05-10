#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "ws2_32.lib")
#include "BaseApp.h"
#include <winsock2.h>
#include <iostream>
#include <string>


// устранение предупреждений
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

// перечисление типов пакетов
enum Packet {
	P_ChatMessage,
	P_Test
};

class MySocket
{
private:
	WSAData wsaData;// структура с датой
	WORD DLLVersion;// версия библиотеки
	SOCKADDR_IN addrSocket;// адрес сокета
	

public:
	SOCKET Connection;// подключение к серверу
	BaseApp* objBase;

	// конструктор класса
	MySocket();

	// загрузка библиотеки сокета
	bool loadLibrary();


	// заполнение информации об адресе сокета
	void setAddresSocket();


	// создание сокета для соединения с сервером
	void createSocket();


	// передача сообщения серверу
	void sendMessageToServer(std::string msg);


	// обработка полученного пакета данных от сервера
	std::string ProcessPacket(Packet packettype);

	void setObjectBase(BaseApp& objBase);

};
