#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

signals:
    void menu();

public:
    explicit Register(DataBase*, QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_backMenu_clicked();

    void on_addRegister_clicked();

    void on_takeAccess_clicked();

    void on_addFaculty_clicked();

    void on_removeFaculty_clicked();

private:
    Ui::Register *ui;
    DataBase *db;
    QMessageBox msgBox;
};

#endif // REGISTER_H
