#include "DBSqliteQt.h"

DBSqliteQt::DBSqliteQt(/*QWidget *parent,*/QString dbName)
{
    // DBSqliteQt db("myDB.db");
    // инициализация БД
    db = QSqlDatabase::addDatabase("QSQLITE");
    // получение имени БД
    db.setDatabaseName(dbName);
    this->parent = parent;
}


// выполнить SQL запрос в БД
bool DBSqliteQt::setQuery(QString sql)
{
    // подключение к БД
    if(!this->db.open()){
        //QMessageBox::warning(this->parent, "Ошибка подключения", "Нет соединения с БД!");
        QApplication::exit();
    }

    QSqlQuery *query = new QSqlQuery(this->db);
    bool result = query->exec(sql);
    // закрытие соединений
    query->clear();
    this->db.close();

    return result;
}


// чтение данных из таблицы БД в список
QList<QList<QString>> DBSqliteQt::readTable(QString sql)
{
    // подключение к БД
    if(!db.open()){
        //QMessageBox::warning(this->parent, "Ошибка подключения", "Нет соединения с БД!");
        QApplication::exit();
    }


    QSqlQuery query;
    QString data = "";
    // колво полей таблицы
    int countFields = db.tables().size();
    // выполнить SQL запрос
    query.exec(sql);

    // список строк таблицы
    QList<QList<QString>> rowsOfTable = QList<QList<QString>>();
    while(query.next()){
        // список значений по всем полям одной строки таблицы
        QList<QString> valuesOfRowTable = QList<QString>();
        // запись значения каждого поля текущей строки в список
        for(int i=0; i<countFields; i++){
            valuesOfRowTable.push_back(query.value(i).toString());
        }
        // добавление в список всех строк таблицы
        rowsOfTable.push_back(valuesOfRowTable);
    }

    // закрытие соединений
    query.clear();
    this->db.close();

    return rowsOfTable;
}


// вставка данных в таблицу
bool DBSqliteQt::insertToTable(QList<QString> fields, QList<QString> values, QString tableName)
{
    // QList<QString> *fields = new QList<QString>();
    // QList<QString> *values = new QList<QString>();
    int countFields = fields.count();
    QString query = "INSERT INTO " + tableName + "(";
    //конкатенация полей таблицы (INSERT)
    for (int i = 0; i < countFields; i++)
    {
        query += fields[i];
        if (i + 1 == countFields) query += ") ";
        else query += ", ";
    }//for

    //добавление к полям значений (VALUES)
    query += "VALUES(";
    for (int i = 0; i < countFields; i++)
    {
        query += "'" + values[i] + "'";
        if (i + 1 == countFields) query += ") ";
        else query += ", ";
    }//for

    // добавление данных в таблицу базы
    return this->setQuery(query);
}

