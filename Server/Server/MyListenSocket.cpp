#include "MyListenSocket.h"

MyListenSocket::MyListenSocket() {
	this->Counter = 0;
	// �������� ����������
	this->createVersion();
	// �������� ������
	this->createAddress();


	// ����� � ����������� ����������
	// ������������� ��������� ���������� � ��������� ���������� ��� ���������� (SOCK_STREAM)
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	// �������� ������ � �������
	bind(sListen, (SOCKADDR*)&addrSocket, sizeof(addrSocket));
	// ������������� ����� � �������� ���������� �� ������� �������
	listen(sListen, SOMAXCONN);
}


// �������� ����������
void MyListenSocket::createVersion() {
	// ������ ������ ���������� winsock ��� �������� ����������
	WORD DLLVersion = MAKEWORD(2, 1);
	// �������� ���������� �������� ������

	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "������ �������� ���������� �������� ������!" << std::endl;
		exit(1);
	}
}


// �������� ������
void MyListenSocket::createAddress()
{
	// ���������� ���������� �� ������ ������
	addrSocket.sin_addr.s_addr = inet_addr("127.0.0.1");// ������ IP �����
	addrSocket.sin_port = htons(1111);// ���� ��� ������������� ��������� � ������������ �������
	addrSocket.sin_family = AF_INET;// ��������� ����������
	sizeofaddr = sizeof(addrSocket);
}


// �������� ����������� � ������� ������ ������
bool MyListenSocket::CreateConnection(int indexConnection)
{
	// addr - �������� ������ ������ �������
	newConnection = accept(sListen, (SOCKADDR*)&addrSocket, &sizeofaddr);
	// �������� ���������� ����������� � �������
	// ����������� ����������� ������� � �������
	if (newConnection == 0) return 0;

	// ���������� ���������� � ������ ����������
	Connections[indexConnection] = newConnection;
	Counter++;
	return 1;
}

// ����� ��������� ��������� �� ������� � �������� ��������� ���� ��������
bool MyListenSocket::ProcessPacket(int index, Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		if (msg_size < 0) return false;
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		// ��������� ��������� �� �������
		recv(Connections[index], msg, msg_size, NULL);

		for (int i = 0; i < Counter; i++) {
			Packet msgtype = P_ChatMessage;// ��� �������� ������
			// ������ �������� ������ ���������� � ��������, �������� ���������� ���������
			// �������� ���� ������ � ���� � ���������� ������� ���������
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			// �������� ���������
			send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		break;
	}
	default:
		std::cout << "�������������� �����: " << packettype << std::endl;
		return false;
		break;
	}

	return true;
}
