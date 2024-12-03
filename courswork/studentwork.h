#ifndef STUDENTWORK_H
#define STUDENTWORK_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class StudentWork;
}

class StudentWork : public QMainWindow
{
    Q_OBJECT

signals:
    void student();

public:
    explicit StudentWork(DataBase*, QWidget *parent = nullptr);
    ~StudentWork();

private slots:
    void on_back_clicked();

    void on_gru_currentTextChanged(const QString& arg);

    void on_moveStuBut_clicked();

    void on_removeStuBut_clicked();

    void on_addStuBut_clicked();

    void on_addActive_clicked();

    void on_cancelActive_clicked();

    void on_stu_currentTextChanged(const QString &arg1);

private:
    Ui::StudentWork *ui;
    QMessageBox msgBox;
    DataBase *db;
};

#endif // STUDENTWORK_H
