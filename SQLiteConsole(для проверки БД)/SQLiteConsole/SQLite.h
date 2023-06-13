#include "sqlite/sqlite3.h"
#include <vector>
#include <string>
using namespace std;

class SQLite {

private:
	sqlite3* db;
	sqlite3_stmt* stmt;// ������������� ������ ��� ������� SELECT �� �������
	char* err;

public:
	
	// ����������� ������
	SQLite(const char *dbName)
	{
		// ����������� � �� � �������� ����� dbName � ������ �������
		sqlite3_open(dbName, &db);
	}

	// SQL ������ � ��
	char* setQuery(string query)
	{
		// ���������� �������
		int result = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
		// ���������� ������ ��� ������ ��������, ������� ��������� ���������� �������
		if (result != SQLITE_OK) return err;
		return NULL;
	}

	// ������ ������ ������� �� � �������� ������ ��������
	vector<vector<string>> readTable(string query, int countFields, string fields[])
	{
		vector<string> vCols;		 // ������
		vector<vector<string>> vRows;// ������� - ������ � �������� �����

		sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
		// ���������� ������ ������ �� ������� � stmt
		while (sqlite3_step(stmt) != SQLITE_DONE)
		{
			int index = 0;
			int value;
			const unsigned char* str;

			for (int i = 0; i < countFields; i++) {
				// ��������� ��� ���� ������� ��� ��� ������
				if (fields[index] == "int") {
					value = sqlite3_column_int(stmt, i);// ������ i-� �������
					// ������ � ������ � ��������������� � ������
					vCols.push_back(to_string(value));
				} 
				if (fields[index] == "txt") {
					str = sqlite3_column_text(stmt, i);// ������ i-� �������
					// ������ � ������ � ��������������� � ������
					vCols.push_back((reinterpret_cast<char const*>(str)));
				}
				
				index++;// ������� � ��������� ������� (����)
			}

			// ������ � ������ �����, ������� ��������
			vRows.push_back(vCols);
			vCols.clear();
		}

		return vRows;
	}
};