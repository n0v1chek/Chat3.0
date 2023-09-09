#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UserInfo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *GetUi(){ return this->ui;}
    QVector<UserInfo>* getAllUsers();
    void setAllUsers(QVector<UserInfo>* allClients);


private slots:
    void on_action_triggered();

    void on_action_3_triggered();

private:
    Ui::MainWindow       *ui;
    QVector<UserInfo>    *allClients;

};
#endif // MAINWINDOW_H
