#include "studentwork.h"
#include "ui_studentwork.h"

StudentWork::StudentWork(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentWork)
{
    ui->setupUi(this);
    this->db = db;
    msgBox.setIcon(QMessageBox::Critical);
    db->inComboBoxGru(ui->gru);
    db->inComboBoxGru(ui->moveNewGru);
    db->inComboBoxGru(ui->pickGru);
}

StudentWork::~StudentWork()
{
    delete ui;
}

void StudentWork::on_back_clicked()
{
    this->close();
    emit student();
}

void StudentWork::on_gru_currentTextChanged(const QString& arg)
{
    db->listStudent(arg, ui->stu);
}



void StudentWork::on_moveStuBut_clicked()
{
    if(db->removeStudent(ui->stu->currentText(), ui->gru->currentText(), ui->moveNewGru->currentText())){
        ui->statusStu->setText("Успешно переведен!");
    }
    else{
        ui->statusStu->setText("Ошибка");
    }
}


void StudentWork::on_removeStuBut_clicked()
{
    if(db->delStudent(ui->stu->currentText(), ui->gru->currentText())){
        ui->statusStu->setText("Успешно отчислен!");
    }
    else{
        ui->statusStu->setText("Ошибка");
    }
}


void StudentWork::on_addStuBut_clicked()
{
    if(db->addStudent(ui->stuSurname->text(), ui->stuName->text(), ui->stuPar->text(), ui->pickGru->currentText())){
        ui->statusStu->setText("Успешно добавлен!");
        ui->stuSurname->clear();
        ui->stuName->clear();
        ui->stuPar->clear();
    }
    else{
        ui->statusStu->setText("Ошибка");
    }
}

void StudentWork::on_addActive_clicked()
{
    if(db->addActStudent(ui->stu->currentText(), ui->gru->currentText())){
        ui->statusStu->setText("Успешно добавленна активность!");
        on_stu_currentTextChanged(ui->stu->currentText());
    }
    else{
        ui->statusStu->setText("Ошибка");
    }
}


void StudentWork::on_cancelActive_clicked()
{
    if(db->canActStudent(ui->stu->currentText(), ui->gru->currentText())){
        ui->statusStu->setText("Успешно убрана активность!");
        on_stu_currentTextChanged(ui->stu->currentText());
    }
    else{
        ui->statusStu->setText("Ошибка");
    }
}

void StudentWork::on_stu_currentTextChanged(const QString &arg1)
{
    if(db->chekAct(arg1, ui->gru->currentText()) == 1){
        ui->cancelActive->setEnabled(1);
        ui->addActive->setEnabled(0);
    }
    else if(db->chekAct(arg1, ui->gru->currentText()) == 2){
        ui->cancelActive->setEnabled(0);
        ui->addActive->setEnabled(1);
    }
    else{
        ui->cancelActive->setEnabled(0);
        ui->addActive->setEnabled(0);
    }
}
