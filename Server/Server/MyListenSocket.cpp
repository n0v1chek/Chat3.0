#include "MyListenSocket.h"


// ����������� ���� ������ Logger
HANDLE Logger::hMutex;
std::ofstream Logger::ofs;// ������ � ����
std::ifstream Logger::ifs;// ������ �� �����
std::string Logger::login, Logger::data;

MyListenSocket::MyListenSocket() {
	db.set("myDB.db");

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

		

		// �������� ����������� ���� ������: (���������� � ������������"user"|���������"msg"|��������� ���������"pm")
		// ��� ������ ���������� �������� ������ � ��
		std::string message(reinterpret_cast<char const*>(msg));
		std::string type = message.substr(0, message.find("_*_"));

		int p = message.find(':');
		int pos1 = type.length() + 3;
		int pos2 = p - pos1;
		int len = message.length() - pos2 - 1 - pos1;

		std::string login = message.substr(pos1, pos2);
		std::string data = message.substr(p+1, message.length() - p);



		if (type == "msg" || type == "pm") {
			// ��������� ������ � ������ ������ ��� ������ � ����
			log.setData(data);
			log.setLogin(login);
			Sleep(5);
		}


		// ��������� � ����� ���
		if (type == "msg") {
			// �������� �������, ���� ��� �� ����������
			char* result = db.setQuery("CREATE TABLE IF NOT EXISTS tchat(ch_sender varchar(30), ch_time DATETIME, ch_msg TEXT);");
			// ������ ��������� � ���
			result = db.setQuery("INSERT INTO tchat(ch_sender, ch_time, ch_msg) VALUES ('" + login + "', DATETIME(), '" + data + "');");
		}
		// ��������� ���������
		if (type == "pm") {
			int pos = login.find('&');
			// ����������
			std::string sender = login.substr(0, pos);
			// �����������
			std::string recipient = login.substr(pos+1, login.length() - pos +1);

			// �������� �������, ���� ��� �� ����������
			char* result = db.setQuery("CREATE TABLE IF NOT EXISTS tprivatechat(ch_sender varchar(30), ch_recipient varchar(30), ch_time DATETIME, ch_msg TEXT);");
			// ������ ��������� � ��������� ���
			result = db.setQuery("INSERT INTO tprivatechat(ch_sender, ch_recipient, ch_time, ch_msg) VALUES ('" + sender + "', '"+ recipient +"', DATETIME(), '" + data + "');");
		}
		// ����������� ������������
		if (type == "user") {
			// �������� �������, ���� ��� �� ����������
			db.setQuery("CREATE TABLE IF NOT EXISTS tusers(u_login varchar(30) unique, u_pass varchar(30));");

			// ������ ������ ������� �� � �������� ������ ��������
			std::string query = "SELECT u_login, u_pass FROM tusers WHERE u_login = '" + login + "'";
			string fields[] = { "u_login", "u_pass" };
			// ���� ������ ������������ � �� ���, �������� ��� � �������
			vector<vector<std::string>> table = db.readTable(query, 3, fields);
			if (table.size() == 0) {
				db.setQuery("INSERT INTO tusers(u_login, u_pass) VALUES ('"+login+"', '"+data+"');");
			}
		}


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
