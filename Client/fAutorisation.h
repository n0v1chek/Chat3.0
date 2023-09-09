#ifndef FAUTORISATION_H
#define FAUTORISATION_H

#include <QDialog>

namespace Ui {
class fAutorisation;
}

class fAutorisation : public QDialog
{
    Q_OBJECT

public:
    explicit fAutorisation(QWidget *parent = nullptr);
    ~fAutorisation();

    void setLogin(QString login);
    void setPass(QString pass);
    QString getLogin();
    QString getPass();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::fAutorisation *ui;
    QString login, pass;
};

#endif // FAUTORISATION_H
