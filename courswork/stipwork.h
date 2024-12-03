#ifndef STIPWORK_H
#define STIPWORK_H

#include <QMainWindow>
#include <QPrinter>
#include "database.h"

namespace Ui {
class stipWork;
}

class stipWork : public QMainWindow
{
    Q_OBJECT

signals:
    void stip();

public:
    explicit stipWork(DataBase*, QWidget *parent = nullptr);
    ~stipWork();

private slots:
    void on_back_clicked();

    void on_pushStudent_clicked();

    void on_print_clicked();

    void on_listFac_currentTextChanged(const QString &arg1);

private:
    Ui::stipWork *ui;
    DataBase *db;
};

#endif // STIPWORK_H
