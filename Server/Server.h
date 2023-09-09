#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QVector>
#include <QTime>
#include "mainwindow.h"
#include "UserInfo.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include "DBSqliteQt.h"



class Server : public QObject
{
    Q_OBJECT
    public:
        explicit Server(Ui::MainWindow *ui, QObject *parent = nullptr);
        void startServer();
        void sendMessageToClients(QString message, QString pmLogin, QTcpSocket *sender);
        QVector<UserInfo>* getAllClients(){ return this->allClients; }
        bool isLoginExists(const QString login, const QString pass);
        bool registrationUserInDb(QString login, QString pass);
        QString getUsersList();

signals:
    public slots:
        void newClientConnection();
        void socketDisconnected();
        void socketReadyRead();
        void socketStateChanged(QAbstractSocket::SocketState state);

private:
        QTcpServer           *chatServer;
        QVector<UserInfo>    *allClients;
        Ui::MainWindow       *ui;
        //DBSqliteQt           sqlite;

};

#endif // SERVER_H
