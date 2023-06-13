#include "sqlite/sqlite3.h"
#include <vector>
#include <string>
using namespace std;

class SQLite {

private:
	sqlite3* db;
	sqlite3_stmt* stmt;// ассоциативный массив при запросе SELECT из таблицы
	char* err;

public:
	
	// конструктор класса
	SQLite(const char *dbName)
	{
		// подключение к БД и создание файла dbName в файлах проекта
		sqlite3_open(dbName, &db);
	}

	// SQL запрос в БД
	char* setQuery(string query)
	{
		// выполнение запроса
		int result = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
		// возвращает ошибку или пустое значение, вслучае успешного выполнения запроса
		if (result != SQLITE_OK) return err;
		return NULL;
	}

	// чтение данных таблицы БД в строчный вектор векторов
	vector<vector<string>> readTable(string query, int countFields, string fields[])
	{
		vector<string> vCols;		 // строки
		vector<vector<string>> vRows;// столбцы - вектор с вектором строк

		sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
		// построчное чтение данных из таблицы в stmt
		while (sqlite3_step(stmt) != SQLITE_DONE)
		{
			int index = 0;
			int value;
			const unsigned char* str;

			for (int i = 0; i < countFields; i++) {
				// определим тип поля таблицы для его чтения
				if (fields[index] == "int") {
					value = sqlite3_column_int(stmt, i);// чтение i-й колонки
					// запись в вектор с преобразованием в строку
					vCols.push_back(to_string(value));
				} 
				if (fields[index] == "txt") {
					str = sqlite3_column_text(stmt, i);// чтение i-й колонки
					// запись в вектор с преобразованием в строку
					vCols.push_back((reinterpret_cast<char const*>(str)));
				}
				
				index++;// переход к следующей колонке (полю)
			}

			// запись в вектор строк, вектора столбцов
			vRows.push_back(vCols);
			vCols.clear();
		}

		return vRows;
	}
};