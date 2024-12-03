#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Authorization)
{
    ui->setupUi(this);
    loginAdmin = "admin";
    loginPass = "admin";
    db = new DataBase();
    registerBut = new Register(db);
    connect(registerBut, &Register::menu, this, &Authorization::show);
    applicationBut = new Application(db);
    connect(applicationBut, &Application::menu, this, &Authorization::show);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_registerButton_clicked()
{
    if(ui->loginChek->text() == loginAdmin and ui->passwordChek->text() == loginPass){
        ui->loginChek->clear();
        ui->passwordChek->clear();
        registerBut->show();
        this->close();
    }
    else{
        ui->loginChek->clear();
        ui->passwordChek->clear();
        ui->Erorr->setText("Неверный логин/пароль");
    }
}

void Authorization::on_enterButton_clicked()
{
    if(db->login(ui->loginChek->text(), ui->passwordChek->text(), this->idUser)){
        ui->loginChek->clear();
        ui->passwordChek->clear();
        applicationBut->show();
        applicationBut->start(idUser);
        this->close();
    }
    else{
        ui->loginChek->clear();
        ui->passwordChek->clear();
        ui->Erorr->setText("Неверный логин/пароль");
    }
}

