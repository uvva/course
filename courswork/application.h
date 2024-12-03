#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include "managementsubjects.h"
#include "database.h"

namespace Ui {
class Application;
}

class Application : public QMainWindow
{
    Q_OBJECT

signals:
    void menu();

public:
    explicit Application(DataBase*, QWidget *parent = nullptr);
    ~Application();
    void start(int);

private slots:
    void on_exit_clicked();

    void on_managementSubject_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listFac_currentTextChanged(const QString &arg1);

private:
    Ui::Application *ui;
    ManagementSubjects *managementsubjectsBut;
    DataBase *db;
    int idUser;
    int dean;
};

#endif // APPLICATION_H
