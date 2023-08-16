#include "MyListenSocket.h"
#include "sqlite/sqlite3.h"
#include <string.h>// библиотека дл€ работы со строками


MyListenSocket ListenSocket;

// обработка сообщений клиентов в параллельном потоке
static void ClientHandler(int index) {
	Packet packettype;

	while (true) {
		recv(ListenSocket.Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		// обработка сообщени€ от клиента
		if (!ListenSocket.ProcessPacket(index, packettype)) break;
	}

	// закрытие сокета дл€ заданного соединени€
	closesocket(ListenSocket.Connections[index]);
	ListenSocket.Connections[index] = 0;
	CloseHandle(ListenSocket.Handle[index]);
	std::cout << "ѕользователь отключилс€ от сервера!\n";
}

// разделение строки по заданному символу на массив подстрок
void splitStrBySymbol(char* str, const char separetor[], int* rows, char words[100][256])
{
	char* strTmp = (char*)malloc(strlen(str) * sizeof(char));
	strcpy(strTmp, str);
	//получение подстроки по символу
	char* tmpStr = strtok(strTmp, separetor);
	// выделение последующих частей строки
	do {
		//запись подстроки в массив
		strcpy(words[*rows], tmpStr);
		(*rows)++;
		// переход к следующей подстроке по заданному символу
		tmpStr = strtok(NULL, separetor);
	} while (tmpStr != NULL);
}




int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "RU");// кириллица вывода в консоль
	std::cout << "—ервер готов к работе и ожидает подключени€...\n";
	


	std::vector<std::string> data = ListenSocket.log.read();
	if (data.size() > 0) {
		std::cout << "Log сервера:\n";
		for (int i = 0; i < data.size(); i++) {
			int size = 0;// колво слов в строке
			char words[1000][256];// объ€вление массива из 1000 слов, каждое длиной до 256 символов
			char tstr[255];
			const char* cstr = data[i].c_str();
			strcpy(tstr, cstr);
			// вызов функции дл€ разделени€ строки на слова
			splitStrBySymbol(tstr, "|", &size, words);// передаем пробел и табул€цию
			std::cout << words[2] << " (" << words[0] << "): " << words[1] << endl;
		}
	}

		

	// поддержка 100 соединений и их прослушка
	for (int i = 0; i < 100; i++) {
		
		// проверка успешности подключени€ к серверу нового сокета
		if (ListenSocket.CreateConnection(i) == 0) {
			std::cout << "Ѕезуспешное подключение пользовател€ к серверу\n";
		}
		else {
			std::cout << "Ќовый пользователь подключилс€ к чату!\n";
			// создание дополнительного потока дл€ клиента
			ListenSocket.Handle[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

			Packet testpacket = P_Test;
			send(ListenSocket.newConnection, (char*)&testpacket, sizeof(Packet), NULL);
		}
	}


	system("pause");
	return 0;
}