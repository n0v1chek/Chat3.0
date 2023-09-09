#include "fAutorisation.h"
#include "ui_fAutorisation.h"
#include <QMessageBox>
#include "UserData.h"

fAutorisation::fAutorisation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fAutorisation)
{
    ui->setupUi(this);
}

fAutorisation::~fAutorisation()
{
    delete ui;
}

void fAutorisation::on_pushButton_2_clicked(){ this->close(); }


void fAutorisation::setLogin(QString login){ this->login = login; }
void fAutorisation::setPass(QString pass){ this->pass = pass; }
QString fAutorisation::getLogin(){ return this->login; }
QString fAutorisation::getPass(){ return this->pass; }

void fAutorisation::on_pushButton_clicked()
{
    QString login = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    if(login == "" || pass == ""){
        QMessageBox::information(this, "Заголовок", "Необходимо заполнить все поля!");
    }else{
        UserData userdata;
        this->setLogin(login);
        this->setPass(pass);
        this->close();
    }
}

