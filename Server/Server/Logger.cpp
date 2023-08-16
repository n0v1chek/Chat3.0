#include "Logger.h"

// получение текущей даты и времени (yyyy.mm.dd hh:mm:ss)
std::string Logger::getCurrentDate() {
	// получение текущей даты и времени
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	// год
	std::string dateTime = std::to_string(systemTime.wYear);
	dateTime += ".";
	// месяц
	if (systemTime.wMonth >= 1 && systemTime.wMonth <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wMonth) + ".";
	// день
	if (systemTime.wDay >= 1 && systemTime.wDay <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wDay) + " ";
	// часы
	if (systemTime.wHour >= 1 && systemTime.wHour <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wHour) + ":";
	// минуты
	if (systemTime.wMinute >= 1 && systemTime.wMinute <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wMinute) + ":";
	// секунды
	if (systemTime.wSecond >= 1 && systemTime.wSecond <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wSecond);

	return dateTime;
}

// запись в файл логов
bool Logger::save(std::string login, std::string data)
{
	if (ofs.fail()) return false;

	// текущая дата и время (yyyy.mm.dd hh:mm:ss)
	std::string dateTime = getCurrentDate();
	// формирование строки для записи
	std::string line = login + "|" + data + "|" + dateTime;
	ofs << line << std::endl;
	return true;
}


static std::string login, data;
// геттер/сеттер для работы с данными
std::string Logger::getLogin() { return login; }
std::string Logger::getData() { return data; }
std::string fileName;


Logger::Logger()
{
	// ДЛЯ РАБОТЫ С ФАЙЛАМИ	
	this->ofs.open("log.txt", std::ios::app);// открытие файла для записи
	this->ifs.open("log.txt", std::ios::in);// открытие файла для чтения
	this->login = "";
	this->data = "";

	// ДЛЯ РАБОТЫ С ПОТОКАМИ
	// создание мьютекса
	hMutex = CreateMutex(NULL, FALSE, NULL);
	// создание нити потока
	hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSave, NULL, 0, &uThrID);
}

Logger::~Logger() {
	this->ofs.close();
	this->ifs.close();
}

// метод параллельного потока
void Logger::ThreadSave() {
	while (true) {
		// останавливает выполнение программы до тех пор пока объект, 
		// идентификатор которого передан в функцию, не окажется в "сигнальном" состоянии
		// ожидание сигнала
		WaitForSingleObject(hMutex, INFINITE);

		std::string s1 = getLogin();
		std::string s2 = getData();
		if (s1 == "" || s2 == "") continue;
		save(s1, s2);
		login = ""; data = "";

		// освобождение нитью объекта мьютекса
		ReleaseMutex(hMutex);
	}
}

// чтение файла логов в вектор
std::vector<std::string> Logger::read()
{
	
	std::string str;
	std::vector<std::string> data;
	if (this->ifs.fail()) return data;
	// чтение каждой строки файла
	while (std::getline(this->ifs, str)) {
		data.push_back(str);
	}
	return data;
}

void Logger::setLogin(std::string login) { this->login = login; }
void Logger::setData(std::string data) { this->data = data; }


