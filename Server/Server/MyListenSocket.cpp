#include "MyListenSocket.h"


// статические поля класса Logger
HANDLE Logger::hMutex;
std::ofstream Logger::ofs;// запись в файл
std::ifstream Logger::ifs;// чтение из файла
std::string Logger::login, Logger::data;

MyListenSocket::MyListenSocket() {
	db.set("myDB.db");

	this->Counter = 0;
	// создание библиотеки
	this->createVersion();
	// создание адреса
	this->createAddress();


	// сокет с результатом выполнения
	// использование семейства протоколов с указанным протоколом для соединения (SOCK_STREAM)
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	// привязка адреса с сокетом
	bind(sListen, (SOCKADDR*)&addrSocket, sizeof(addrSocket));
	// прослушивание порта в ожидании соединения со стороны клиента
	listen(sListen, SOMAXCONN);
}


// создание библиотеки
void MyListenSocket::createVersion() {
	// запрос версии библиотеки winsock для загрузки библиотеки
	WORD DLLVersion = MAKEWORD(2, 1);
	// загрузка библиотеки заданной версии

	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Ошибка загрузки библиотеки заданной версии!" << std::endl;
		exit(1);
	}
}


// создание адреса
void MyListenSocket::createAddress()
{
	// заполнение информации об адресе сокета
	addrSocket.sin_addr.s_addr = inet_addr("127.0.0.1");// хранит IP адрес
	addrSocket.sin_port = htons(1111);// порт для идентификации программы с поступающими данными
	addrSocket.sin_family = AF_INET;// семейство протоколов
	sizeofaddr = sizeof(addrSocket);
}


// создание подключения к серверу нового сокета
bool MyListenSocket::CreateConnection(int indexConnection)
{
	// addr - содержит данные адреса клиента
	newConnection = accept(sListen, (SOCKADDR*)&addrSocket, &sizeofaddr);
	// проверка успешности подключения к серверу
	// безуспешное подключение клиента к серверу
	if (newConnection == 0) return 0;

	// добавление соединения в массив соединений
	Connections[indexConnection] = newConnection;
	Counter++;
	return 1;
}


// пакет обработки сообщения от клиента и рассылка сообщения всем клиентам
bool MyListenSocket::ProcessPacket(int index, Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		if (msg_size < 0) return false;
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		// получение сообщение от клиента
		recv(Connections[index], msg, msg_size, NULL);

		

		// ПРОВЕРКА ПОЛУЧЕННОГО ТИПА ДАННЫХ: (ИНФОРМАЦИЯ О ПОЛЬЗОВАТЕЛЕ"user"|СООБЩЕНИЕ"msg"|ПРИВАТНОЕ СООБЩЕНИЕ"pm")
		// ДЛЯ ЗАПИСИ ПОЛУЧЕННЫХ СЕРВЕРОМ ДАННЫХ В БД
		std::string message(reinterpret_cast<char const*>(msg));
		std::string type = message.substr(0, message.find("_*_"));

		int p = message.find(':');
		int pos1 = type.length() + 3;
		int pos2 = p - pos1;
		int len = message.length() - pos2 - 1 - pos1;

		std::string login = message.substr(pos1, pos2);
		std::string data = message.substr(p+1, message.length() - p);



		if (type == "msg" || type == "pm") {
			// установка данных в объект класса для записи в файл
			log.setData(data);
			log.setLogin(login);
			Sleep(5);
		}


		// сообщение в общий чат
		if (type == "msg") {
			// создание таблицы, если она не существует
			char* result = db.setQuery("CREATE TABLE IF NOT EXISTS tchat(ch_sender varchar(30), ch_time DATETIME, ch_msg TEXT);");
			// запись сообщения в чат
			result = db.setQuery("INSERT INTO tchat(ch_sender, ch_time, ch_msg) VALUES ('" + login + "', DATETIME(), '" + data + "');");
		}
		// приватное сообщение
		if (type == "pm") {
			int pos = login.find('&');
			// получатель
			std::string sender = login.substr(0, pos);
			// отправитель
			std::string recipient = login.substr(pos+1, login.length() - pos +1);

			// создание таблицы, если она не существует
			char* result = db.setQuery("CREATE TABLE IF NOT EXISTS tprivatechat(ch_sender varchar(30), ch_recipient varchar(30), ch_time DATETIME, ch_msg TEXT);");
			// запись сообщения в приватный чат
			result = db.setQuery("INSERT INTO tprivatechat(ch_sender, ch_recipient, ch_time, ch_msg) VALUES ('" + sender + "', '"+ recipient +"', DATETIME(), '" + data + "');");
		}
		// регистрация пользователя
		if (type == "user") {
			// создание таблицы, если она не существует
			db.setQuery("CREATE TABLE IF NOT EXISTS tusers(u_login varchar(30) unique, u_pass varchar(30));");

			// чтение данных таблицы БД в строчный вектор векторов
			std::string query = "SELECT u_login, u_pass FROM tusers WHERE u_login = '" + login + "'";
			string fields[] = { "u_login", "u_pass" };
			// если такого пользователя в БД нет, записать его в таблицу
			vector<vector<std::string>> table = db.readTable(query, 3, fields);
			if (table.size() == 0) {
				db.setQuery("INSERT INTO tusers(u_login, u_pass) VALUES ('"+login+"', '"+data+"');");
			}
		}


		for (int i = 0; i < Counter; i++) {
			Packet msgtype = P_ChatMessage;// тип передачи данных
			// первый параметр хранит соединения с клиентом, которому отправляем сообщение
			// передача типа данных и типа и подготовка размера сообщения
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			// передача сообщения
			send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		break;
	}
	default:
		std::cout << "Нераспознанный пакет: " << packettype << std::endl;
		return false;
		break;
	}

	return true;
}
