#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>

class Logger
{
	// поля класса

	/*
		Объекты-события используются для уведомления ожидающих нитей о
		наступлении какого-либо события
	*/
	// переменные для работы с потоками
	HANDLE hThr;// идентификатор нити
	unsigned long uThrID;

	// для работы с файлами
	static std::ofstream ofs;// запись в файл
	static std::ifstream ifs;// чтение из файла


	// получение текущей даты и времени (yyyy.mm.dd hh:mm:ss)
	static std::string getCurrentDate();

	// запись в файл логов
	static bool save(std::string login, std::string data);


	static std::string login, data;
	// геттер/сеттер для работы с данными
	static std::string getLogin();
	static std::string getData();



public:

	static HANDLE hMutex;// идентификатор мьютекса


	Logger();

	~Logger();

	// метод параллельного потока
	static void ThreadSave();

	// чтение файла логов в вектор
	std::vector<std::string> read();

	void setLogin(std::string login);
	void setData(std::string data);

};
