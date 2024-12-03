#include "objectwork.h"
#include "ui_objectwork.h"

ObjectWork::ObjectWork(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ObjectWork)
{
    ui->setupUi(this);
    this->db = db;
    msgBox.setIcon(QMessageBox::Critical);
    db->inComboBox(ui->removeSubject, 1);
    db->inComboBox(ui->renameSubject, 1);
    db->inComboBox(ui->listSubject, 1);
}

ObjectWork::~ObjectWork()
{
    delete ui;
}

void ObjectWork::on_back_clicked()
{
    this->close();
    emit object();
}

void ObjectWork::on_addSubjectBut_clicked()
{
    if(!(db->addObject(ui->addSubject->text(), ui->credit->isChecked()))){
        msgBox.setText("Такую предметную область нельзя добавить!");
        msgBox.exec();
    }
    else{
        ui->status->setText("Успешно добавлено!");
        db->checkObject();
        ui->addSubject->clear();
    }
}

void ObjectWork::on_removeSubjectBut_clicked()
{
    QString cmd;
    if(ui->removeSubject->currentIndex() != 0){
        cmd = db->removeQuaObject(ui->removeSubject->currentText());
    }
    else{
        cmd = "";
    }
    if(cmd != ""){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(cmd);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int answer = msgBox.exec();
        switch (answer) {
        case QMessageBox::Yes:
            db->removeObject(ui->removeSubject->currentText());
            db->checkObject();
        }
    }
    else{
        msgBox.setText("Такую предметную область нельзя убрать!");
        msgBox.exec();
    }
}

void ObjectWork::on_renameSubjectBut_clicked()
{
    if(ui->renameSubject->currentIndex() == 0 or !(db->renameObject(ui->renameSubject->currentText(), ui->newNameSubject->text()))){
        msgBox.setText("Такую предметную область нельзя переименовать!");
        msgBox.exec();
    }
    else{
        ui->status->setText("Успешно изменено!");
        db->checkObject();
        ui->newNameSubject->clear();
    }
}

void ObjectWork::on_listSubjectBut_clicked()
{
    db->listObject(ui->listSubject->currentText(), ui->tableName);
}
