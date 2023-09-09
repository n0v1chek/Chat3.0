#include "Server.h"


Server::Server(Ui::MainWindow *ui, QObject *parent) : QObject(parent){ this->ui = ui; }



// ЗАПУСК СЕРВЕРА
void Server::startServer()
{
    // вектор всех подключенных клиентов на сервер
    allClients = new QVector<UserInfo>;
    // создание QTcpServer объекта
    chatServer = new QTcpServer();
    // установка лимита на количество подключений клиентов
    chatServer->setMaxPendingConnections(100);
    // сигнал срабатывает каждый раз при подключении нового клиента к серверу
    connect(chatServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));


    // слушать подключения по установленному порту
    if (chatServer->listen(QHostAddress::Any, 6000))
    {
        ui->logField->append("Сервер работает по порту 6000.");
    }
    else
    {
        ui->logField->append("Неудалось запустить сервер. Error: " + chatServer->errorString());
    }
}


// ОТПРАВКА СООБЩЕНИЙ КЛИЕНТАМ
void Server::sendMessageToClients(QString message, QString pmLogin, QTcpSocket *sender)
{
    // передача сообщения всем открытым клиентам
    for (int i = 0; i < allClients->size(); i++)
    {
        if ((*allClients)[i].getSocket()->isOpen() && (*allClients)[i].getSocket()->isWritable())
        {
            // отправить всем пользователям
            if(pmLogin == "") (*allClients)[i].setSocket(message);
            else{
                // отправить только отправителю и получателю (приватное сообщение)
                if((*allClients)[i].getLogin() == pmLogin || (*allClients)[i].getSocket() == sender) (*allClients)[i].setSocket(message);
            }
        }
    }
}


// ПОДКЛЮЧЕНИЕ НОВОГО КЛИЕНТА
void Server::newClientConnection()
{
    // подключение клиента к серверу через сокет
    QTcpSocket* client = chatServer->nextPendingConnection();
    // получение информации о клиенте
    QString socketIpAddress = client->peerAddress().toString();
    // подключение сигналов к соответствующим функциям слота
    // отключение клиента от сервера
    connect(client, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
    // отправка клиентом сообщения
    connect(client, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    // подключение другого сигнала изминения состояния сокета
    connect(client, &QTcpSocket::stateChanged, this, &Server::socketStateChanged);
    // добавление нового клиента в вектор клиентов
    UserInfo infoUser("", client);
    allClients->push_back(infoUser);
}


// ОТКЛЮЧЕНИЕ КЛИЕНТА ОТ СЕРВЕРА
void Server::socketDisconnected()
{
    // получение информации о пользователе, для вывода информации о нем на экран
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    //QString socketIpAddress = client->peerAddress().toString();

    QString login = "";
    // поиск нужного сокета
    for (int i = 0; i < allClients->size(); i++){
        if((*allClients)[i].getSocket() == client){
            login = (*allClients)[i].getLogin();
            (*allClients).remove(i);
            break;
        }
    }

    if(login != ""){
        QString message = "<font color=\"Red\">Пользователь с ником " + login + " покинул чат.</font>";
        ui->logField->append(message);
        // отправка сообщения пользователям о покинувшем чат пользователе
        sendMessageToClients(message, "", nullptr);
    }


    // удаление пользователя из списка пользователей чата
    int countUsers = ui->usersList->count();
    for(int i = 0; i < countUsers; i++){
        if(ui->usersList->item(i)->text() == login){
            delete ui->usersList->takeItem(i);
            break;
        }
    }
}


// ПРИЕМНИК И ОБРАБОТКА СООБЩЕНИЙ ОТ КЛИЕНТОВ
void Server::socketReadyRead()
{
    // получение информации о пользователе
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    // IP addres пользователя
    QString socketIpAddress = client->peerAddress().toString();
    //int port = client->peerPort();// порт
    // сообщение от клиента
    QString message = QString(client->readAll());
    QString pmLogin = "";// логин для приватного сообщения


    // подключение пользователя к чату
    if(message.indexOf("присоединился к чату") >= 0){
        // получение логина пользователя
        // замена подстроки в строке
        QString loginUser(message);
        QString subStr("<font color=\"Purple\">Пользователь с ником ");
        QString newStr("");
        loginUser.replace(loginUser.indexOf(subStr), subStr.size(), newStr);
        // замена подстроки в строке
        subStr = " присоединился к чату.</font>";
        loginUser.replace(loginUser.indexOf(subStr), subStr.size(), newStr);


        // разделить строку на логин и пароль
        QStringList loginPass = loginUser.split("_*_");
        message.replace(message.indexOf(loginUser), loginUser.size(), loginPass[0]);
        loginUser = loginPass[0];
        QString pass = loginPass[1];


        // регистрация пользователя и авторизация пользователя
        int inx = allClients->size()-1;
        if(this->registrationUserInDb(loginUser, pass)){
            // добавление логина пользователя в список пользователей
            (*allClients)[inx].setLogin(loginUser);
            (*allClients)[inx].setPass(pass);
            ui->usersList->addItem(loginUser);
            ui->logField->append("<font color=\"Green\">Подключение нового клиента.</font>");

            // отправка клиентам обновленный список пользователей
            QString userList = "updateUsersList_*_" + getUsersList() + "|||";
            sendMessageToClients(userList, "", client);

        }else{
            // неудалось авторизироваться, отлючиться от сервера
            // отключение клиента от сервера
            QTcpSocket* socket = (*this->allClients)[inx].getSocket();
            socket->close();
            return;
        }
    }


    // обычное или приватное сообщение пользователям(ю)
    else{
        // проверка приватного сообщения
        QStringList data = message.split(":: ");
        data = data[0].split(">: ");
        // если такой пользователь существует
        if(data.count() > 1){
            if(isLoginExists(data[1], "")){
                pmLogin = data[1];
            }
        }
        if(pmLogin != "") message = "Приватное сообщение от пользователя: " + message;
        else message = "Сообщение от пользователя: " + message;
    }



    // если сообщение приватное, необходимо убрать ник получателя
    if(pmLogin != ""){
        QString subStr(pmLogin+":: ");
        message.replace(message.indexOf(subStr), subStr.size(), "");
    }

    // вывод сообщения от пользователя в лог сервера
    ui->logField->append(message);
    // отправка текущего сообщения всем клиентам сервера или отправка приватного сообщения
    sendMessageToClients(message, pmLogin, client);
}


// РЕГИСТРАЦИЯ И АВТОРИЗАЦИЯ ПОЛЬЗОВАТЕЛЯ В ТАБЛИЦЕ БД
bool Server::registrationUserInDb(QString login, QString pass)
{
    // проверка существования логина в базе и системе
    DBSqliteQt db("myDB.db");


    // создание таблицы, если она не существует
    db.setQuery("CREATE TABLE IF NOT EXISTS tusers(u_login varchar(30) unique, u_pass varchar(30))");
    // попытка поиска пользователя по заданным логину и паролю
    QList<QList<QString>> checkUserInDb = db.readTable("SELECT u_login, u_pass FROM tusers WHERE u_login = '"+login+"' AND u_pass = '"+ pass +"'");


    // зашедший пользователь не числится в таблице БД, записать его в БД
    if(checkUserInDb.size() == 0){
        // подготовка данных для записи в таблицу БД данных о новом пользователе
        QList<QString> *fields = new QList<QString>();
        fields->push_back("u_login");
        fields->push_back("u_pass");

        QList<QString> *values = new QList<QString>();
        values->push_back(login);
        values->push_back(pass);

        // вставка данных в таблицу
        DBSqliteQt db("myDB.db");
        bool resultRegistration = db.insertToTable((*fields), (*values), "tusers");       
        // записать данные в таблицу неудалось
        if(resultRegistration == false) return false;
    }


    // если пользователь не авторизирован в системе, авторизоваться
    if(!this->isLoginExists(login, pass)) return true;
    // пользователь уже авторизирован в системе
    else return false;
}


// ВОЗВРАЩАЕТ СПИСОК ПОДКЛЮЧЕННЫХ КЛЛИЕНТОВ СЕРВЕРА
QString Server::getUsersList()
{
    QString users = "";
    for (int i = 0; i < allClients->size(); i++)
    {
        if(users != "") users += "_*_";
        users += (*allClients)[i].getLogin();
    }
    return users;
}

// поиск пользователя в системе по логину и паролю
bool Server::isLoginExists(const QString login, const QString pass){
    for (int i = 0; i < allClients->size(); i++)
    {
        if((*allClients)[i].getLogin() == login){
            // необходимо проверить не только логин, но и пароль
            if(pass != ""){
                if((*allClients)[i].getPass() == pass) return true;
                else return false;
            }else return true;
        }
    }
    return false;
}


// ВЫЗОВ ФУНКЦИИ ПРИ ПРИ КАЖДОМ ИЗМЕНЕНИИ СОСТОЯНИЯ СЕТИ КЛИЕНТА (ПОДКЛЮЧЕНИЕ, ОТКЛЮЧЕНИЕ, ПРОСЛУШИВАНИЕ)
void Server::socketStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    //int port = client->peerPort();
    QString desc;
    // вывод сообщения
    if (state == QAbstractSocket::UnconnectedState)     desc = "Сокет не подключен.";
    else if (state == QAbstractSocket::HostLookupState) desc = "Сокет выполняет поиск имени хоста.";
    else if (state == QAbstractSocket::ConnectingState) desc = "Сокет устанавливает соединение.";
    else if (state == QAbstractSocket::ConnectedState)  desc = "Подключение установлено.";
    else if (state == QAbstractSocket::BoundState)      desc = "Сокет привязан к адресу и порту.";
    else if (state == QAbstractSocket::ClosingState)    desc = "Сокет скоро закроется (возможно, данные все еще ожидают записи).";
    else if (state == QAbstractSocket::ListeningState)  desc = "Только для внутреннего использования.";

    //ui->logField->append("Состояние сокета изменено (" + socketIpAddress + ":" + QString::number(port) + "): " + desc);
}




