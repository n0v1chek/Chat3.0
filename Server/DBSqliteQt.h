#ifndef DBSQLITEQT_H
#define DBSQLITEQT_H

// QT       += sql
// БД (database.db) должна находиться на этапе разработки в build-Server-Desktop_Qt_6_2_0_MinGW_64_bit-Debug
// а уже после реализации, в папке Release
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include <QApplication>
#include <QList>


class DBSqliteQt
{
public:
    DBSqliteQt(/*QWidget *parent, */QString dbName);
    // выполнить SQL запрос в БД
    bool setQuery(QString sql);
    // чтение данных из таблицы БД в список
    QList<QList<QString>> readTable(QString sql);
    // вставка данных в таблицу
    bool insertToTable(QList<QString> fields, QList<QString> values, QString tableName);

private:
    QSqlDatabase db;// объект для работы с БД
    QWidget *parent;// указатель на родительскую форму
};

#endif // DBSQLITEQT_H
