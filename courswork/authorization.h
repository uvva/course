#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMainWindow>
#include <QtSql>
#include "register.h"
#include "database.h"
#include "application.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Authorization;
}
QT_END_NAMESPACE

class Authorization : public QMainWindow
{
    Q_OBJECT

public:
    Authorization(QWidget *parent = nullptr);
    ~Authorization();

private slots:
    void on_registerButton_clicked();

    void on_enterButton_clicked();

private:
    Ui::Authorization *ui;
    Application *applicationBut;
    Register *registerBut;
    DataBase *db;
    int idUser;
    QString loginAdmin;
    QString loginPass;
};
#endif // AUTHORIZATION_H
