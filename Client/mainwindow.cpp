#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include "fAutorisation.h"


QString userName = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedToHost = false;
    ui->chatDisplay->append("Нет соединения с сервером...");
}


MainWindow::~MainWindow(){ delete ui; }


// СОЕДИНЕНИЕ С СОКЕТОМ
void MainWindow::socketConnected()
{
    printMessage("<font color=\"Green\">Подключение к серверу.</font>");
    socket->write("<font color=\"Purple\">Пользователь с ником " + userData.GetLogin().toUtf8()+"_*_"+userData.GetPass().toUtf8() + " присоединился к чату.</font>");
    connectedToHost = true;
}


// ОТКЛЮЧЕНИЕ ОТ СЕРВЕРА
void MainWindow::socketDisconnected() { this->closeConnection(); }


// удаление пользователя из списка пользователей чата
void MainWindow::deleteUserFromListUsers(QString loginUser){
    int countUsers = ui->usersList->count();
    if(countUsers > 0){
        // удаление логина из списка клиентов
        for(int i=0; i<countUsers; i++){
            QString login = ui->usersList->item(i)->text();
            if(loginUser == login){
                delete ui->usersList->item(i);
                break;
            }
        }
    }
}




// ОБРАБОТЧИК ПОЛУЧЕННЫХ ОТ КЛИЕНТА СООБЩЕНИЙ (СЛОТ СИГНАЛ)
void MainWindow::socketReadyRead()
{
    QString message = socket->readAll();
    // удаление пользователя из списка пользователей
    if(message.indexOf(" покинул чат.") >= 0){
        // получение логина пользователя
        // замена подстроки в строке
        QString loginUser(message);
        QString subStr("<font color=\"Red\">Пользователь с ником ");
        QString newStr("");
        loginUser.replace(loginUser.indexOf(subStr), subStr.size(), newStr);
        // замена подстроки в строке
        subStr = " покинул чат.</font>";
        loginUser.replace(loginUser.indexOf(subStr), subStr.size(), newStr);

        // удаление пользователя из списка пользователей чата
        deleteUserFromListUsers(loginUser);
        //socket->write(message.toUtf8());
    }

    // актуальный список пользователей для загрузки
    if(message.indexOf("updateUsersList") >= 0){
        // встроке указан список клиентов и через символ ||| сообщение о присоединившемся к чату пользователю
        QStringList temp = message.split("|||");
        message = temp[1];

        // обновление списка пользователей чата
        QStringList users = temp[0].split("_*_");
        ui->usersList->clear();
        for(int i=1; i<users.size(); i++){
            ui->usersList->addItem(users[i]);
        }
    }

    // вывод сообщения на экран
    ui->chatDisplay->append(message);
}

// ВЫВОД СООБЩЕНИЯ НА ЭКРАН
void MainWindow::printMessage(QString message) { ui->chatDisplay->append(message); }


void MainWindow::on_pushButton_clicked(){ sendMessage(); }


void MainWindow::on_messageInput_returnPressed(){ sendMessage(); }


// ОТПРАВКА СООБЩЕНИЯ НА СЕРВЕР
void MainWindow::sendMessage()
{
    if (connectedToHost){
        QString message = ui->messageInput->text();
        socket->write(QTime::currentTime().toString().toUtf8() + " <font color=\"Blue\">" + userName.toUtf8() + "</font>: " + message.toUtf8());
        ui->messageInput->clear();
    }
}


// ОБРАБОТЧИК ОТКЛЮЧЕНИЯ ОТ СЕРВЕРА
void  MainWindow::closeConnection()
{
    if(connectedToHost){
        ui->chatDisplay->setText("");
        ui->messageInput->setText("");
        socket->disconnectFromHost();
        userName = "";
        connectedToHost = false;
        this->setWindowTitle("Программа - Чат");
        ui->usersList->clear();
        printMessage("<font color=\"Red\">Отключение от сервера.</font>");
    }
}


void MainWindow::on_MainWindow_destroyed(){ this->closeConnection(); }


// СОБЫТИЕ ЗАКРЫТИЯ ОКНА ЧАТА
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn =
        QMessageBox::question( this, "",
        tr("Вы точно хотите выйти?\n"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        this->closeConnection();
        event->accept();
    }
}


void MainWindow::on_action_4_triggered() { QApplication::exit(); }


// АВТОРИЗАЦИЯ ПОЛЬЗОВАТЕЛЯ
void MainWindow::on_action_triggered()
{
    if(connectedToHost){
        QMessageBox::information(this, "Информация", "Необходимо выйти из учетной записи!");
        return;
    }
    fAutorisation window;
    // модальное окно
    window.setModal(true);
    // открыть окно
    window.exec();


    QString login = window.getLogin();
    QString pass = window.getPass();


    if(login != "" && pass != ""){
        //QMessageBox::information(this, "", "Авторизация пользователя "+ login +" прошла успешно!");
        //QString msg = "user_*_" + login + ":" + pass;
        userData.SetLogin(login);
        userData.SetPass(pass);
        //this->sendToServer(msg);
    }
    else if(login == "" && pass == ""){}
    else QMessageBox::warning(this, "Ошибка авторизации", "Пользователя нет в БД.");


    // ЕСЛИ АВТОРИЗАЦИЯ ПРОШЛА УСПЕШНО
    userName = login;
    ui->chatDisplay->setText("");
    ui->messageInput->setText("");

    // СОЕДИНЕНИЕ С СЕРВЕРОМ
    // если соединение не установлено, установить его
    if (!connectedToHost)
    {
        this->setWindowTitle("Программа - Чат: "+userName);

        // создание сокета для соединения с портом 6000 и IP
        socket = new QTcpSocket();
        // connected the socket object to its respective slot functions when connected(),disconnected(), and readReady() signals were triggered.
        connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

        // подключение к серверу
        socket->connectToHost("127.0.0.1", 6000);
    }
}


// ВЫХОД ИЗ УЧЕТНОЙ ЗАПИСИ
void MainWindow::on_action_3_triggered() { this->closeConnection(); }



// ЛИЧНОЕ СООБЩЕНИЕ
void MainWindow::on_action_6_triggered()
{
    // определить логин выбранного пользователя
    if(ui->usersList->count() > 0){
        QList<QListWidgetItem*> items = ui->usersList->selectedItems();
        if(items.count() > 0){
            // выбранный логин пользователя
            QString login = ui->usersList->currentItem()->text();
            if(login == userName){
                QMessageBox::information(this, "Информация", "Нельзя отправлять сообщения самому себе!");
                return;
            }
            ui->messageInput->clear();
            ui->messageInput->setText(login+":: ");
            ui->messageInput->setFocus();
        }else QMessageBox::information(this, "Информация", "Необходимо выбрать логин пользователя из списка!");
    }
}



