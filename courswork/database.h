#ifndef DATABASE_H
#define DATABASE_H
#define SQL_NAME "..\\table\\courseProgram.db"

#include <QtSql>
#include <QTableView>
#include <QStandardItemModel>

class DataBase
{

public:
    DataBase();
    bool login(const QString&, const QString&, int&);
    bool takeAccess(const QString&, const QString&);
    bool checkFaculty(QString, bool);
    int registr(const QString&, const QString&, const QString&, QString, const int&);
    QString outFullName(const int&);
    QString outInterfaceObject(const int&, int&);
    QString cause(const QString&);
    void inComboBox(QComboBox*, int);
    void inComboBoxGru(QComboBox*);
    void inComboBoxFac(QComboBox*);
    void intTable(QTableView*);
    void clearTable();
    QString chekTable(int, const QString&);
    void listStudent(int, const QString&);
    bool addActStudent(const QString&, const QString&);
    bool canActStudent(const QString&, const QString&);

    void addFaculty(QString);
    void removeFaculty(QString);
    bool addObject(QString, bool);
    void checkObject();
    QString removeQuaObject(const QString&);
    void removeObject(const QString&);
    bool renameObject(const QString&, QString);
    void listObject(const QString&, QTableView*);
    void tabDeb(QTableView*,const QString&, double);

    void checkGru();

    void checkFac();

    bool renameGro(const QString&, const QString&);
    bool addGro(const QString&, const QString&);
    bool addSubGro(const QString&, const QString&);
    bool delSubGro(const QString&, const QString&);
    QString removeQuaGroup(const QString&);
    void removeGroup(const QString&);
    void checkGruForApp(const QString&, QComboBox*);

    void listStudent(const QString&, QComboBox*);
    bool removeStudent(const QString&, const QString&, const QString&);
    bool delStudent(const QString&, const QString&);
    bool addStudent(const QString&, const QString&, const QString&, const QString&);
    int chekAct(const QString&, const QString&);

private:
    struct Obj{
        int idCmd;
        QComboBox* cbObj;
    };

    std::vector<Obj> cbObj;
    std::vector<QComboBox*> cbGru;
    std::vector<QComboBox*> cbFac;
    QSqlDatabase base;

    struct Student{
        double pr;
        QString name_gr;
        QString full_name;
        QString cause;
    };

    QTableView* tableStu;

    bool checkFullName(QString&);

};

#endif // DATABASE_H
