#pragma once
#pragma comment(lib, "ws2_32.lib")// ��� ������� � ��������� ��������
#include <winsock2.h>
#include "SQLite.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "Logger.h"



// ���������� ��������������
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

enum Packet {
	P_ChatMessage,
	P_Test
};




class MyListenSocket
{
private:
	WSAData wsaData;// ���������� � ������ � ������ ���������� winsock
	


public:
	SOCKET Connections[100];// ������ ����������
	int Counter;// ���������� ���������� � ��������
	HANDLE Handle[100];// ������������� ������
	Logger log;

	// ���������� ���������� �� ������ ������
	SOCKADDR_IN addrSocket;
	int sizeofaddr;

	// ����� � ����������� ����������
	// ������������� ��������� ���������� � ��������� ���������� ��� ���������� (SOCK_STREAM)
	SOCKET sListen;
	// ����� �����������
	SOCKET newConnection;
	SQLite db;


	// ����������� ������
	MyListenSocket();

	// �������� ����������
	void createVersion();

	// �������� ������
	void createAddress();

	// �������� ����������� � ������� ������ ������
	bool CreateConnection(int indexConnection);

	// ����� ��������� ��������� �� ������� � �������� ��������� ���� ��������
	bool ProcessPacket(int index, Packet packettype);

};
