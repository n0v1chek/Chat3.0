#include "UserInfo.h"

UserInfo::UserInfo(QString login, QTcpSocket *socket)
{
    this->login = login;
    this->socket = socket;
}

void UserInfo::setLogin(const QString login){ this->login = login; }
void UserInfo::setPass(const QString pass) { this->password = pass; }

QString UserInfo::getLogin(){ return this->login; }
QString UserInfo::getPass(){ return this->password; }
QTcpSocket* UserInfo::getSocket(){ return this->socket; }
void UserInfo::setSocket(QString message){ this->socket->write(message.toUtf8()); }
