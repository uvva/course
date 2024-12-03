#include "register.h"
#include "ui_register.h"

Register::Register(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register)
{
    this->db = db;
    ui->setupUi(this);
    db->inComboBox(ui->subject, 0);
}

Register::~Register()
{
    delete ui;
}

void Register::on_backMenu_clicked()
{
    this->close();
    emit menu();
}

void Register::on_addRegister_clicked()
{
    msgBox.setIcon(QMessageBox::Critical);
    int cmd = (db->registr(ui->login->text(), ui->pass->text(), ui->subject->currentText(), ui->full_name->text(), 0));
    if(cmd == 1){
        msgBox.setText("Пользователь уже есть!");
        msgBox.exec();
    }
    else if(cmd == 2){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(db->cause(ui->login->text()));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int cmd = msgBox.exec();
        switch (cmd) {
        case QMessageBox::Yes:
            db->registr(ui->login->text(), ui->pass->text(), ui->subject->currentText(), ui->full_name->text(), 1);
            break;
        }
    }
    else if(cmd == 3){
        msgBox.setText("Некорректные поля!");
        msgBox.exec();
    }
    ui->login->clear();
    ui->pass->clear();
    ui->full_name->clear();
}

void Register::on_takeAccess_clicked()
{
    msgBox.setIcon(QMessageBox::Information);
    if(db->takeAccess(ui->login->text(), ui->reasAccess->toPlainText())){
        msgBox.setText("У пользователя забран доступ!");
        msgBox.exec();
    }
    else{
        msgBox.setText("Пользователь не найден!");
        msgBox.exec();
    }
    ui->login->clear();
    ui->reasAccess->clear();
}


void Register::on_addFaculty_clicked()
{
    msgBox.setIcon(QMessageBox::Critical);
    if(db->checkFaculty(ui->facultyName->text(), 1)){
        db->addFaculty(ui->facultyName->text());
        ui->facultyName->clear();
    }
    else{
        msgBox.setText("Название занято!");
        msgBox.exec();
    }
}


void Register::on_removeFaculty_clicked()
{
    msgBox.setIcon(QMessageBox::Critical);
    if(db->checkFaculty(ui->facultyName->text(), 0)){
        db->removeFaculty(ui->facultyName->text());
        ui->facultyName->clear();
    }
    else{
        msgBox.setText("Такого факультета нет!");
        msgBox.exec();
    }
}

