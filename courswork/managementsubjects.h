#ifndef MANAGEMENTSUBJECTS_H
#define MANAGEMENTSUBJECTS_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPrinter>
#include "objectwork.h"
#include "studentwork.h"
#include "stipwork.h"
#include "groupwork.h"
#include "database.h"

namespace Ui {
class ManagementSubjects;
}

class ManagementSubjects : public QMainWindow
{
    Q_OBJECT

signals:
    void app();

public:
    explicit ManagementSubjects(DataBase*, QWidget *parent = nullptr);
    ~ManagementSubjects();

private slots:
    void on_backMenu_clicked();

    void on_objectWork_clicked();

    void on_groupWork_clicked();

    void on_studentWork_clicked();

    void on_stipWork_clicked();

private:
    Ui::ManagementSubjects *ui;
    DataBase *db;

    ObjectWork *objectWorkBut;
    StudentWork *studentWorkBut;
    stipWork *stipWorkBut;
    GroupWork *groupWorkBut;
};

#endif // MANAGEMENTSUBJECTS_H
