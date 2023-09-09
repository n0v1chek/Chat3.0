#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QTcpSocket>

class UserInfo
{
public:
    UserInfo(QString login, QTcpSocket *socket);
    void setLogin(const QString login);
    void setPass(const QString pass);
    QString getLogin();
    QString getPass();
    QTcpSocket* getSocket();
    void setSocket(QString message);


private:
    QString login, password;
    QTcpSocket* socket;
};

#endif // USERINFO_H
