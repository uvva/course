#ifndef OBJECTWORK_H
#define OBJECTWORK_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class ObjectWork;
}

class ObjectWork : public QMainWindow
{
    Q_OBJECT

signals:
    void object();

public:
    explicit ObjectWork(DataBase*, QWidget *parent = nullptr);
    ~ObjectWork();

private slots:
    void on_back_clicked();

    void on_addSubjectBut_clicked();

    void on_renameSubjectBut_clicked();

    void on_removeSubjectBut_clicked();

    void on_listSubjectBut_clicked();

private:
    Ui::ObjectWork *ui;
    QMessageBox msgBox;
    DataBase *db;
};

#endif // OBJECTWORK_H
