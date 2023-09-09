#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTime>
#include "UserData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_connectButton_clicked();
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();


    //void on_sendButton_clicked();

    void on_pushButton_clicked();

    void on_messageInput_returnPressed();

    void on_MainWindow_destroyed();

    void on_action_4_triggered();

    void on_action_triggered();

    void on_action_3_triggered();

    void on_action_6_triggered();

private:
    Ui::MainWindow  *ui;
    bool            connectedToHost;
    QTcpSocket      *socket;
    UserData userData;

    void printMessage(QString message);
    void closeConnection();
    void closeEvent(QCloseEvent *event);
    void sendMessage();
    // удаление пользователя из списка пользователей чата
    void deleteUserFromListUsers(QString loginUser);

};
#endif // MAINWINDOW_H
