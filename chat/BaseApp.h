#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "UserData.h"



class BaseApp
{
private:
    BaseApp();

public:
    BaseApp(BaseApp const&) = delete;
    BaseApp& operator= (BaseApp const&) = delete;
    ~BaseApp() = default;

public:
    static BaseApp* instance();

    UserData* getCurrent() const;
    void setCurrent(UserData* userData);

    void addUser(const UserData& ud);
    bool isLogin(const std::string& login) const;
    bool isPassword(const std::string& password) const;

    bool verifyUserData(const std::string& login, const std::string& password);

    UserData* findUser(const std::string& login);

    void printChat(const std::string& chatName);                           // print concrete chat
    void printChat();                                                      // print general chat
    void sendMessage(const Message& message, const std::string& receiver); // send to concrete chat
    void sendMessage(const Message& message);                              // send to general chat
    void addGeneralChat(const Message& message);

    
private:
    static std::unique_ptr<BaseApp> _instance;
    std::vector<Message> _generalChat;
    std::unordered_map<std::string, UserData,SHA1> _usersData; // unordered_map of users
    UserData* _currentUser;
    
    
};