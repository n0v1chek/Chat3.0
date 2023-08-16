#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>

class Logger
{
	// ���� ������

	/*
		�������-������� ������������ ��� ����������� ��������� ����� �
		����������� ������-���� �������
	*/
	// ���������� ��� ������ � ��������
	HANDLE hThr;// ������������� ����
	unsigned long uThrID;

	// ��� ������ � �������
	static std::ofstream ofs;// ������ � ����
	static std::ifstream ifs;// ������ �� �����


	// ��������� ������� ���� � ������� (yyyy.mm.dd hh:mm:ss)
	static std::string getCurrentDate();

	// ������ � ���� �����
	static bool save(std::string login, std::string data);


	static std::string login, data;
	// ������/������ ��� ������ � �������
	static std::string getLogin();
	static std::string getData();



public:

	static HANDLE hMutex;// ������������� ��������


	Logger();

	~Logger();

	// ����� ������������� ������
	static void ThreadSave();

	// ������ ����� ����� � ������
	std::vector<std::string> read();

	void setLogin(std::string login);
	void setData(std::string data);

};
