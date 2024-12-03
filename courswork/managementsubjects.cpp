#include "managementsubjects.h"
#include "ui_managementsubjects.h"

ManagementSubjects::ManagementSubjects(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagementSubjects)
{
    ui->setupUi(this);
    this->db = db;
    objectWorkBut = new ObjectWork(db);
    connect(objectWorkBut, &ObjectWork::object, this, &ManagementSubjects::show);
    studentWorkBut = new StudentWork(db);
    connect(studentWorkBut, &StudentWork::student, this, &ManagementSubjects::show);
    stipWorkBut = new stipWork(db);
    connect(stipWorkBut, &stipWork::stip, this, &ManagementSubjects::show);
    groupWorkBut = new GroupWork(db);
    connect(groupWorkBut, &GroupWork::group, this, &ManagementSubjects::show);
}

ManagementSubjects::~ManagementSubjects()
{
    delete ui;
}

void ManagementSubjects::on_backMenu_clicked()
{
    this->close();
    emit app();
}

void ManagementSubjects::on_objectWork_clicked()
{
    objectWorkBut->show();
    this->close();
}


void ManagementSubjects::on_groupWork_clicked()
{
    groupWorkBut->show();
    this->close();
}


void ManagementSubjects::on_studentWork_clicked()
{
    studentWorkBut->show();
    this->close();
}


void ManagementSubjects::on_stipWork_clicked()
{
    stipWorkBut->show();
    this->close();
}

