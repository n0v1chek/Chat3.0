#ifndef USERDATA_H
#define USERDATA_H
#include <QString>

class UserData
{
private:
    QString login;
    QString password;
public:
    UserData();
    QString GetLogin();
    QString GetPass();
    void SetLogin(QString login);
    void SetPass(QString pass);
    UserData(const QString login, const QString password);
};



#endif // USERDATA_H
