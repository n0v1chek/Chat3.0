#include "mainwindow.h"
#include <QApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Server* server = new Server(w.GetUi());
    server->startServer();
    w.setAllUsers(server->getAllClients());

    return a.exec();
}
