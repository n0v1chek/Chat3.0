#include "BaseApp.h"
#include "MySocket.h"
MySocket Socket;


// обработка ответов сервера в параллельном потоке
void ClientHandler() {
    Packet packettype;
    while (true) {
        // чтение ответа
        recv(Socket.Connection, (char*)&packettype, sizeof(Packet), NULL);
        // обработка полученного пакета данных
        std::string msg = Socket.ProcessPacket(packettype);
        if (msg != "") {
            
            // проверка полученного типа данных: (информация о пользователе|сообщение)
            std::string type = msg.substr(0, msg.find("_*_"));

            int p = msg.find(':');
            int pos1 = type.length() + 3;
            int pos2 = p - pos1;
            int len = msg.length() - pos2 - 1 - pos1;

            std::string login = msg.substr(pos1, pos2);
            std::string data  = msg.substr(p + 1, msg.length() - p);


            // запись информации о новом пользователе в чат
            if (type == "user") {
                // проверка налачия записи о пользователи
                UserData* find = Socket.objBase->findUser(login);
                if (find == NULL) {
                    Socket.objBase->addUser(UserData(login, data));
                }
            }

            // получение сообщения в общий чат 
            if (type == "msg") {
                // проверка логина текущего пользователя
                UserData* current = Socket.objBase->getCurrent();
                if (current->getLogin() != login) {
                    Socket.objBase->addGeneralChat(Message(login, data));
                }
            }

            // получение сообщения пользователю - получателю (логин отправителя, логин получателя, сообщение)
            if (type == "pm") {
                // добавление сообщения для пользователей: receiver/sender
                int splitPos = login.find("&");
                std::string sender = login.substr(0, splitPos);
                std::string receiver = login.substr(splitPos+1, login.length() - splitPos);

                // проверка логина текущего пользователя
                UserData* current = Socket.objBase->getCurrent();
                if (current->getLogin() != sender) {
                    Socket.objBase->findUser(sender)->getMessages()[receiver].push_back(Message(sender, data));
                    Socket.objBase->findUser(receiver)->getMessages()[sender].push_back(Message(sender, data));
                }
            }
        }
    }
    // закрытие соединения с сервером
    closesocket(Socket.Connection);
}


std::unique_ptr<BaseApp> BaseApp::_instance = nullptr;

void BaseApp::addUser(const UserData& ud)
{
    _usersData[ud.getLogin()] = ud;

    // отправка данных о новом пользователе на сервер (логин, пароль)
    std::string msg = "user_*_" + ud.getLogin() + ":" + ud.getPassword();
    Socket.sendMessageToServer(msg);
}

// отправка сообщения от пользователя к пользователю
void BaseApp::sendMessage(const Message& message, const std::string& receiver)
{
    std::string sender = _currentUser->getLogin();
    // добавление сообщения для пользователей: receiver/sender
    findUser(sender)->getMessages()[receiver].push_back(message);
    findUser(receiver)->getMessages()[sender].push_back(message);

    // отправка данных на сервер (логин отправителя, логин получателя, сообщение)
    std::string msg = "pm_*_" + sender + "&" + receiver + ":" + message.getMessage();
    Socket.sendMessageToServer(msg);
}

// отправка сообщения от пользователя в общий чат
void BaseApp::sendMessage(const Message& message)
{
    _generalChat.push_back(message);
    
    // отправка данных на сервер (логин, сообщение)
    std::string msg = "msg_*_" + message.getName() + ":" + message.getMessage();
    Socket.sendMessageToServer(msg);
}



BaseApp* BaseApp::instance()
{
    if (!_instance)
    {
        _instance = static_cast<std::unique_ptr<BaseApp>>(new BaseApp);
    }
    return _instance.get();
}

UserData* BaseApp::getCurrent() const
{
    return _currentUser;
}

void BaseApp::setCurrent(UserData* userData)
{
    _currentUser = userData;
}

bool BaseApp::isLogin(const std::string& login) const
{
    if (_usersData.find(login) != _usersData.end())
        return true;

    return false;
}

bool BaseApp::isPassword(const std::string& password) const
{

    for (const auto& i : _usersData)
    {
        if (i.second.getPassword() == password)
            return true;
    }

    return false;
}

bool BaseApp::verifyUserData(const std::string& login, const std::string& password) {
    if (_usersData.find(login) == _usersData.end())
        return false;
    else if (_usersData.at(login).getPassword() == password)
        return true;
    else return false;
}

UserData* BaseApp::findUser(const std::string& login)
{
    auto ud = _usersData.find(login);

    if (ud != _usersData.end())
        return &ud->second;

    return nullptr;
}

void BaseApp::printChat(const std::string& chatName)
{
    std::system("cls");

    auto messages = _currentUser->getMessages()[chatName];

    if (messages.empty())
    {
        std::cout << "This chat is empty, send him a message!\n";
    }
    else
    {
        for (int i = 0; i < messages.size(); ++i)
        {
            std::cout << messages[i].getName() << ": " << messages[i].getMessage() << "\n";
        }
    }
    

    std::system("pause");
}

void BaseApp::printChat()
{
    std::system("cls");


    if (_generalChat.size() == 0)
    {
        std::cout << "This chat is empty, send him a message!\n";
    }
    else
    {
        for (int i = 0; i < _generalChat.size(); ++i)
        {
            std::cout << _generalChat[i].getName() << ": " << _generalChat[i].getMessage() << "\n";
        }
    }
    std::system("pause");
}

void BaseApp::addGeneralChat(const Message& message)
{
    this->_generalChat.push_back(message);
}

// конструктор без параметров
BaseApp::BaseApp() 
    : _usersData(), _currentUser(nullptr), _generalChat()
{
    // проверка подключения к серверу
    if (Socket.Connection == NULL) {
        std::cout << "Ошибка: безуспешное подключение к серверу.\n";
        system("pause");
        exit(1);
    }
    
    // передача ссылки на объект класса BaseApp
    Socket.setObjectBase(*this);

    // создание потока, для обработки ответов сервера
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
}