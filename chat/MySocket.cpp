#include "MySocket.h"

// конструктор класса
MySocket::MySocket() {
	// если загрузка библиотеки сокета успешно, выполняем подключение
	if (this->loadLibrary()) {
		// заполнение информации об адресе сокета
		this->setAddresSocket();
		// создание сокета для соединения с сервером
		this->createSocket();
	}
}

// загрузка библиотеки сокета
bool MySocket::loadLibrary()
{
	// запрос версии библиотеки winsock для загрузки библиотеки
	this->DLLVersion = MAKEWORD(2, 1);
	// загрузка библиотеки заданной версии
	if (WSAStartup(this->DLLVersion, &this->wsaData) != 0) return false;
	return true;
}


// заполнение информации об адресе сокета
void MySocket::setAddresSocket()
{
	int sizeofaddr = sizeof(this->addrSocket);
	this->addrSocket.sin_addr.s_addr = inet_addr("127.0.0.1");// хранит IP адрес
	this->addrSocket.sin_port = htons(1111);// порт для идентификации программы с поступающими данными
	this->addrSocket.sin_family = AF_INET;// семейство протоколов
}


// создание сокета для соединения с сервером
void MySocket::createSocket()
{
	this->Connection = socket(AF_INET, SOCK_STREAM, NULL);
	// попытка подключения к серверу
	if (connect(this->Connection, (SOCKADDR*)&this->addrSocket, sizeof(this->addrSocket)) != 0) this->Connection = NULL;
}


// передача сообщения серверу
void MySocket::sendMessageToServer(std::string msg)
{
	// размер сообщения
	int msg_size = msg.size();
	// тип передачи данных
	Packet packettype = P_ChatMessage;
	// первый параметр хранит соединения с клиентом, которому отправляем сообщение
	// передача типа данных и типа и подготовка размера сообщения
	send(this->Connection, (char*)&packettype, sizeof(Packet), NULL);
	send(this->Connection, (char*)&msg_size, sizeof(int), NULL);
	// передача сообщения
	send(this->Connection, msg.c_str(), msg_size, NULL);
	Sleep(10);
}


// обработка полученного пакета данных от сервера
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
		//std::cout << "Выполнен тест пакета данных...\n";
		break;
	default:
		std::cout << "Нераспознанный пакет: " << packettype << std::endl;
		break;
	}

	return "";
}

void MySocket::setObjectBase(BaseApp& objBase)
{
	this->objBase = (&objBase);
}