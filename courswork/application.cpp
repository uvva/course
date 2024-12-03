#include "application.h"
#include "ui_application.h"

Application::Application(DataBase* db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Application)
{
    ui->setupUi(this);
    this->db = db;
    db->inComboBox(ui->choiceSub, 1);
    db->intTable(ui->tableStudent);
    managementsubjectsBut = new ManagementSubjects(db);
    connect(managementsubjectsBut, &ManagementSubjects::app, this, &Application::show);
    db->inComboBoxFac(ui->listFac);
    db->checkFac();
    db->checkGruForApp(ui->listFac->currentText(), ui->listGr);
}

Application::~Application()
{
    delete ui;
}

void Application::start(int idUser){
    this->idUser = idUser;
    ui->hello->setText(db->outFullName(this->idUser));
    ui->subject->setText(db->outInterfaceObject(this->idUser, dean));
    ui->managementSubject->setEnabled(!(dean - 1));
    ui->choiceSub->setEnabled(!(dean - 1));
    this->idUser = dean;
}

void Application::on_exit_clicked()
{
    db->clearTable();
    this->close();
    emit menu();
}


void Application::on_managementSubject_clicked()
{
    managementsubjectsBut->show();
    this->close();
}


void Application::on_pushButton_clicked()
{
    db->listStudent(dean, ui->choiceSub->currentText());
}


void Application::on_pushButton_2_clicked()
{
    ui->status->setText(db->chekTable(dean, ui->choiceSub->currentText()));
}


void Application::on_listFac_currentTextChanged(const QString &arg)
{
    db->checkGruForApp(arg, ui->listGr);
}

