#include <iostream>
#include <Windows.h>
#include "CliBaseApp.h"


auto main() -> int
{
	setlocale(LC_ALL, "RU");// ����� � ������� ���������
	SetConsoleCP(1251);// ��������� ������� �������� win-cp 1251 � ����� �����
	SetConsoleOutputCP(1251); // ��������� ������� �������� win-cp 1251 � ����� ������



	CLIBaseApp cliBaseApp;

	while (true)
	{
		std::system("cls");
		cliBaseApp.help();

		int answer;
		std::cin >> answer;

		switch (answer)
		{
		case 1:
			cliBaseApp.signIn();
			break;

		case 2:
			cliBaseApp.signUp();
			break;

		case 0:

			return 0;
		default:
			break;
		}
	}
}