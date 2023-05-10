#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "ws2_32.lib")
#include "BaseApp.h"
#include <winsock2.h>
#include <iostream>
#include <string>


// ���������� ��������������
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

// ������������ ����� �������
enum Packet {
	P_ChatMessage,
	P_Test
};

class MySocket
{
private:
	WSAData wsaData;// ��������� � �����
	WORD DLLVersion;// ������ ����������
	SOCKADDR_IN addrSocket;// ����� ������
	

public:
	SOCKET Connection;// ����������� � �������
	BaseApp* objBase;

	// ����������� ������
	MySocket();

	// �������� ���������� ������
	bool loadLibrary();


	// ���������� ���������� �� ������ ������
	void setAddresSocket();


	// �������� ������ ��� ���������� � ��������
	void createSocket();


	// �������� ��������� �������
	void sendMessageToServer(std::string msg);


	// ��������� ����������� ������ ������ �� �������
	std::string ProcessPacket(Packet packettype);

	void setObjectBase(BaseApp& objBase);

};
