#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include "UserInfo.h"
#include <QMessageBox>







MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}


// ПОЛУЧЕНИЕ СПИСКА КЛИЕНТОВ ДЛЯ ОБМЕНА С КЛАССОМ SERVER
QVector<UserInfo>* MainWindow::getAllUsers(){ return this->allClients; }
void MainWindow::setAllUsers(QVector<UserInfo>* allClients){
    this->allClients = allClients;
}


// ОТКЛЮЧИТЬ ВЫБРАННОГО ПОЛЬЗОВАТЕЛЯ ОТ ЧАТА
void MainWindow::on_action_triggered()
{
    if(ui->usersList->count() > 0){
        // удаление логина из списка клиентов
        //QModelIndexList index = ui->usersList->selectionModel()->selectedIndexes();
        QList<QListWidgetItem*> items = ui->usersList->selectedItems();
        if(items.count() > 0){
            // отключение клиента от сервера
            QString login = ui->usersList->currentItem()->text();
            // передача сообщения все открытым клиентам
            for (int i = 0; i < allClients->size(); i++){
                if((*allClients)[i].getLogin() == login){
                    QTcpSocket* socket = (*this->allClients)[i].getSocket();
                    socket->close();
                    break;
                }
            }
        }else QMessageBox::information(this, "Информация", "Необходимо выбрать логин пользователя из списка для его удаления!");
    }else QMessageBox::information(this, "Информация", "Нет доступных подключений!");
}


// ЗАВЕРШИТЬ РАБОТУ СЕРВЕРА
void MainWindow::on_action_3_triggered() { QApplication::exit(); }

