#include "MyListenSocket.h"
#include "sqlite/sqlite3.h"
#include <string.h>// ���������� ��� ������ �� ��������


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

// ���������� ������ �� ��������� ������� �� ������ ��������
void splitStrBySymbol(char* str, const char separetor[], int* rows, char words[100][256])
{
	char* strTmp = (char*)malloc(strlen(str) * sizeof(char));
	strcpy(strTmp, str);
	//��������� ��������� �� �������
	char* tmpStr = strtok(strTmp, separetor);
	// ��������� ����������� ������ ������
	do {
		//������ ��������� � ������
		strcpy(words[*rows], tmpStr);
		(*rows)++;
		// ������� � ��������� ��������� �� ��������� �������
		tmpStr = strtok(NULL, separetor);
	} while (tmpStr != NULL);
}




int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "RU");// ��������� ������ � �������
	std::cout << "������ ����� � ������ � ������� �����������...\n";
	


	std::vector<std::string> data = ListenSocket.log.read();
	if (data.size() > 0) {
		std::cout << "Log �������:\n";
		for (int i = 0; i < data.size(); i++) {
			int size = 0;// ����� ���� � ������
			char words[1000][256];// ���������� ������� �� 1000 ����, ������ ������ �� 256 ��������
			char tstr[255];
			const char* cstr = data[i].c_str();
			strcpy(tstr, cstr);
			// ����� ������� ��� ���������� ������ �� �����
			splitStrBySymbol(tstr, "|", &size, words);// �������� ������ � ���������
			std::cout << words[2] << " (" << words[0] << "): " << words[1] << endl;
		}
	}

		

	// ��������� 100 ���������� � �� ���������
	for (int i = 0; i < 100; i++) {
		
		// �������� ���������� ����������� � ������� ������ ������
		if (ListenSocket.CreateConnection(i) == 0) {
			std::cout << "����������� ����������� ������������ � �������\n";
		}
		else {
			std::cout << "����� ������������ ����������� � ����!\n";
			// �������� ��������������� ������ ��� �������
			ListenSocket.Handle[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

			Packet testpacket = P_Test;
			send(ListenSocket.newConnection, (char*)&testpacket, sizeof(Packet), NULL);
		}
	}


	system("pause");
	return 0;
}