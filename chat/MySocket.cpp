#include "MySocket.h"

// ����������� ������
MySocket::MySocket() {
	// ���� �������� ���������� ������ �������, ��������� �����������
	if (this->loadLibrary()) {
		// ���������� ���������� �� ������ ������
		this->setAddresSocket();
		// �������� ������ ��� ���������� � ��������
		this->createSocket();
	}
}

// �������� ���������� ������
bool MySocket::loadLibrary()
{
	// ������ ������ ���������� winsock ��� �������� ����������
	this->DLLVersion = MAKEWORD(2, 1);
	// �������� ���������� �������� ������
	if (WSAStartup(this->DLLVersion, &this->wsaData) != 0) return false;
	return true;
}


// ���������� ���������� �� ������ ������
void MySocket::setAddresSocket()
{
	int sizeofaddr = sizeof(this->addrSocket);
	this->addrSocket.sin_addr.s_addr = inet_addr("127.0.0.1");// ������ IP �����
	this->addrSocket.sin_port = htons(1111);// ���� ��� ������������� ��������� � ������������ �������
	this->addrSocket.sin_family = AF_INET;// ��������� ����������
}


// �������� ������ ��� ���������� � ��������
void MySocket::createSocket()
{
	this->Connection = socket(AF_INET, SOCK_STREAM, NULL);
	// ������� ����������� � �������
	if (connect(this->Connection, (SOCKADDR*)&this->addrSocket, sizeof(this->addrSocket)) != 0) this->Connection = NULL;
}


// �������� ��������� �������
void MySocket::sendMessageToServer(std::string msg)
{
	// ������ ���������
	int msg_size = msg.size();
	// ��� �������� ������
	Packet packettype = P_ChatMessage;
	// ������ �������� ������ ���������� � ��������, �������� ���������� ���������
	// �������� ���� ������ � ���� � ���������� ������� ���������
	send(this->Connection, (char*)&packettype, sizeof(Packet), NULL);
	send(this->Connection, (char*)&msg_size, sizeof(int), NULL);
	// �������� ���������
	send(this->Connection, msg.c_str(), msg_size, NULL);
	Sleep(10);
}


// ��������� ����������� ������ ������ �� �������
std::string MySocket::ProcessPacket(Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(this->Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(this->Connection, msg, msg_size, NULL);
		std::string data = msg;
		delete[] msg;
		return data;
		break;
	}
	case P_Test:
		//std::cout << "�������� ���� ������ ������...\n";
		break;
	default:
		std::cout << "�������������� �����: " << packettype << std::endl;
		break;
	}

	return "";
}

void MySocket::setObjectBase(BaseApp& objBase)
{
	this->objBase = (&objBase);
}