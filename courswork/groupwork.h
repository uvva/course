#ifndef GROUPWORK_H
#define GROUPWORK_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class GroupWork;
}

class GroupWork : public QMainWindow
{
    Q_OBJECT

signals:
    void group();

public:
    explicit GroupWork(DataBase*, QWidget *parent = nullptr);
    ~GroupWork();

private slots:
    void on_back_clicked();

    void on_renameGroupBut_clicked();

    void on_addGroupBut_clicked();

    void on_listGroupBut_clicked();

    void on_listGroupBut_2_clicked();

    void on_removeGroupBut_clicked();

private:
    Ui::GroupWork *ui;
    QMessageBox msgBox;
    DataBase *db;
};

#endif // GROUPWORK_H
