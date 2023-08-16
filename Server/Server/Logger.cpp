#include "Logger.h"

// ��������� ������� ���� � ������� (yyyy.mm.dd hh:mm:ss)
std::string Logger::getCurrentDate() {
	// ��������� ������� ���� � �������
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	// ���
	std::string dateTime = std::to_string(systemTime.wYear);
	dateTime += ".";
	// �����
	if (systemTime.wMonth >= 1 && systemTime.wMonth <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wMonth) + ".";
	// ����
	if (systemTime.wDay >= 1 && systemTime.wDay <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wDay) + " ";
	// ����
	if (systemTime.wHour >= 1 && systemTime.wHour <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wHour) + ":";
	// ������
	if (systemTime.wMinute >= 1 && systemTime.wMinute <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wMinute) + ":";
	// �������
	if (systemTime.wSecond >= 1 && systemTime.wSecond <= 9) dateTime += "0";
	dateTime += std::to_string(systemTime.wSecond);

	return dateTime;
}

// ������ � ���� �����
bool Logger::save(std::string login, std::string data)
{
	if (ofs.fail()) return false;

	// ������� ���� � ����� (yyyy.mm.dd hh:mm:ss)
	std::string dateTime = getCurrentDate();
	// ������������ ������ ��� ������
	std::string line = login + "|" + data + "|" + dateTime;
	ofs << line << std::endl;
	return true;
}


static std::string login, data;
// ������/������ ��� ������ � �������
std::string Logger::getLogin() { return login; }
std::string Logger::getData() { return data; }
std::string fileName;


Logger::Logger()
{
	// ��� ������ � �������	
	this->ofs.open("log.txt", std::ios::app);// �������� ����� ��� ������
	this->ifs.open("log.txt", std::ios::in);// �������� ����� ��� ������
	this->login = "";
	this->data = "";

	// ��� ������ � ��������
	// �������� ��������
	hMutex = CreateMutex(NULL, FALSE, NULL);
	// �������� ���� ������
	hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSave, NULL, 0, &uThrID);
}

Logger::~Logger() {
	this->ofs.close();
	this->ifs.close();
}

// ����� ������������� ������
void Logger::ThreadSave() {
	while (true) {
		// ������������� ���������� ��������� �� ��� ��� ���� ������, 
		// ������������� �������� ������� � �������, �� �������� � "����������" ���������
		// �������� �������
		WaitForSingleObject(hMutex, INFINITE);

		std::string s1 = getLogin();
		std::string s2 = getData();
		if (s1 == "" || s2 == "") continue;
		save(s1, s2);
		login = ""; data = "";

		// ������������ ����� ������� ��������
		ReleaseMutex(hMutex);
	}
}

// ������ ����� ����� � ������
std::vector<std::string> Logger::read()
{
	
	std::string str;
	std::vector<std::string> data;
	if (this->ifs.fail()) return data;
	// ������ ������ ������ �����
	while (std::getline(this->ifs, str)) {
		data.push_back(str);
	}
	return data;
}

void Logger::setLogin(std::string login) { this->login = login; }
void Logger::setData(std::string data) { this->data = data; }


