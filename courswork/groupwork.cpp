#include "groupwork.h"
#include "ui_groupwork.h"

GroupWork::GroupWork(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GroupWork)
{
    ui->setupUi(this);
    this->db = db;
    msgBox.setIcon(QMessageBox::Critical);
    db->inComboBox(ui->listObject, 1);
    db->inComboBox(ui->listObject_2, 1);
    db->checkObject();
    db->inComboBoxGru(ui->listGroup);
    db->inComboBoxGru(ui->renameGroup);
    db->inComboBoxGru(ui->removeGroup);
    db->checkGru();
    db->inComboBoxFac(ui->listFac);
    db->checkFac();
}

GroupWork::~GroupWork()
{
    delete ui;
}

void GroupWork::on_back_clicked()
{
    this->close();
    emit group();
}

void GroupWork::on_renameGroupBut_clicked()
{
    if(db->renameGro(ui->renameGroup->currentText(), ui->newNameGroup->text())){
        ui->statusGroup->setText("Группа успешно переименована!");
        ui->newNameGroup->clear();
    }
    else{
        ui->statusGroup->setText("Ошибка");
    }
}


void GroupWork::on_addGroupBut_clicked()
{
    if(db->addGro(ui->listFac->currentText(), ui->addGroup->text())){
        ui->statusGroup->setText("Группа успешно добавлена!");
        ui->addGroup->clear();
    }
    else{
        ui->statusGroup->setText("Ошибка");
    }
}


void GroupWork::on_listGroupBut_clicked()
{
    if(db->addSubGro(ui->listGroup->currentText(), ui->listObject->currentText())){
        ui->statusGroup->setText("Успешно изменено!");
    }
    else{
        ui->statusGroup->setText("Ошибка");
    }
}


void GroupWork::on_listGroupBut_2_clicked()
{
    if(db->delSubGro(ui->listGroup->currentText(), ui->listObject_2->currentText())){
        ui->statusGroup->setText("Успешно изменено!");
    }
    else{
        ui->statusGroup->setText("Ошибка");
    }
}


void GroupWork::on_removeGroupBut_clicked()
{
    QString cmd;
    cmd = db->removeQuaGroup(ui->removeGroup->currentText());
    if(cmd.size() == 0){
        ui->statusGroup->setText("Ошибка");
    }
    else{
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(cmd);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int answer = msgBox.exec();
        switch (answer) {
        case QMessageBox::Yes:
            db->removeGroup(ui->removeGroup->currentText());
        }
    }
}
