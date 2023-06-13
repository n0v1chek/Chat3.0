#include <iostream>
#include "SQLite.h"
using namespace std;


int main()
{
	setlocale(LC_ALL, "RU");
	SQLite db("myDB.db");


	
	// получение данных из таблицы
	string fields[] = {"txt", "txt", "txt", "txt" };


	cout << "\nТАБЛИЦА - пользователи:\n";
	// чтение данных таблицы БД в строчный вектор векторов
	vector<vector<string>> data = db.readTable("SELECT u_login, u_pass FROM tusers", 2, fields);
	for (int i = 0; i < data.size(); i++) {
		cout << data[i][0] << ": " << data[i][1] << endl;
	}


	cout << "\nТАБЛИЦА - Приватный чат:\n";
	// чтение данных таблицы БД в строчный вектор векторов
	data = db.readTable("SELECT ch_sender, ch_recipient, ch_time, ch_msg FROM tprivatechat", 4, fields);
	for (int i = 0; i < data.size(); i++) {
		cout << data[i][0] << " -> " << data[i][1] << " (" << data[i][2] << "): " << data[i][3] << endl;
	}
	
	cout << "\nТАБЛИЦА - Общий чат:\n";
	// чтение данных таблицы БД в строчный вектор векторов
	data = db.readTable("SELECT ch_sender, ch_time, ch_msg FROM tchat", 3, fields);
	for (int i = 0; i < data.size(); i++) {
		cout << data[i][1] << ":(" << data[i][0] << ") " << data[i][2] << endl;
	}
	
	cout << endl;
	system("pause");
	return 0;
}