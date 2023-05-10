#include "MyListenSocket.h"
MyListenSocket ListenSocket;

// ��������� ��������� �������� � ������������ ������
static void ClientHandler(int index) {
	Packet packettype;

	while (true) {
		recv(ListenSocket.Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		// ��������� ��������� �� �������
		if (!ListenSocket.ProcessPacket(index, packettype)) break;
	}

	// �������� ������ ��� ��������� ����������
	closesocket(ListenSocket.Connections[index]);
	ListenSocket.Connections[index] = 0;
	CloseHandle(ListenSocket.Handle[index]);
	std::cout << "������������ ���������� �� �������!\n";
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "RU");// ��������� ������ � �������
	
	std::cout << "������ ����� � ������ � ������� �����������...\n";
	// ��������� 100 ���������� � �� ���������
	for (int i = 0; i < 100; i++) {
		
		// �������� ���������� ����������� � ������� ������ ������
		if (ListenSocket.CreateConnection(i) == 0) {
			std::cout << "����������� ����������� ������������ � �������\n";
		}
		else {
			std::cout << "����� ������������ ����������� � ����!\n";
			//ListenSocket.SendMessageToCLient();
			// �������� ��������������� ������ ��� �������
			ListenSocket.Handle[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

			Packet testpacket = P_Test;
			send(ListenSocket.newConnection, (char*)&testpacket, sizeof(Packet), NULL);
		}
	}


	system("pause");
	return 0;
}