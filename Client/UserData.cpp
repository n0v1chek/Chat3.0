#include "UserData.h"

UserData::UserData(){
    this->login = "";
    this->password = "";
}

QString UserData::GetLogin(){ return this->login; }

QString UserData::GetPass(){ return this->password; }

UserData::UserData(const QString login, const QString password){
    this->login = login;
    this->password = password;
}

void UserData::SetLogin(QString login){ this->login = login; }
void UserData::SetPass(QString pass){ this->password = pass; }

