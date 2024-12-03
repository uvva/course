#include "database.h"

DataBase::DataBase() {
    base = QSqlDatabase::addDatabase("QSQLITE");
    base.setDatabaseName(SQL_NAME);
    if(!base.open()){
        qDebug("Неудалось найти базу данных");
    }
    else{
        QSqlQuery start;
        start.exec("PRAGMA foreign_keys = ON");
    }
}

void DataBase::inComboBox(QComboBox* cb, int idCmd){ //добавление предметных
    Obj obj;
    obj.cbObj = cb;
    obj.idCmd = idCmd;
    this->cbObj.push_back(obj);
}

void DataBase::inComboBoxGru(QComboBox* cb){
    this->cbGru.push_back(cb);
}

void DataBase::inComboBoxFac(QComboBox* cb){
    this->cbFac.push_back(cb);
}

void DataBase::checkObject(){
    QSqlQuery start;
    for(size_t count = 0; count < cbObj.size(); count++){
        cbObj[count].cbObj->clear();
    }
    if(start.exec("SELECT * FROM object")){
        while(start.next()){
            for(size_t count = 0; count < cbObj.size(); count++){
                if(cbObj[count].idCmd and start.value(1).toString() != "Деканат"){
                    cbObj[count].cbObj->addItem(start.value(1).toString());
                }
                else if(!(cbObj[count].idCmd)){
                    cbObj[count].cbObj->addItem(start.value(1).toString());
                }
            }
        }
    }
}

void DataBase::checkGru(){
    QSqlQuery start;
    for(size_t count = 0; count < cbGru.size(); count++){
        cbGru[count]->clear();
    }
    if(start.exec("SELECT * FROM groups WHERE id_groups IS NOT NULL")){
        while(start.next()){
            for(size_t count = 0; count < cbGru.size(); count++){
                cbGru[count]->addItem(start.value(1).toString());
            }
        }
    }
}

void DataBase::checkGruForApp(const QString& facul, QComboBox* cb){
    QSqlQuery check;
    cb->clear();
    check.prepare(("SELECT * FROM groups 'gr' JOIN groups 'gr2' ON gr.id_groups = gr2.id WHERE gr2.name = :facul"));
    check.bindValue(":facul", facul);
    check.exec();
    while(check.next()){
        cb->addItem(check.value(1).toString());
    }
}

void DataBase::checkFac(){
    QSqlQuery start;
    for(size_t count = 0; count < cbFac.size(); count++){
        cbFac[count]->clear();
    }
    if(start.exec("SELECT * FROM groups WHERE id_groups IS NULL")){
        while(start.next()){
            for(size_t count = 0; count < cbFac.size(); count++){
                cbFac[count]->addItem(start.value(1).toString());
            }
        }
    }
}

void DataBase::listStudent(const QString& arg, QComboBox* cb){
    QSqlQuery start;
    cb->clear();
    if(start.exec("SELECT student.surname, student.name, student.patronymic FROM student JOIN groups ON groups.id = student.id_groups WHERE groups.name = '"+arg+"'")){
        while(start.next()){
            QString full_name = start.value(0).toString() + ' ' + start.value(1).toString() + ' ' + start.value(2).toString();
            cb->addItem(full_name);
        }
    }
}

QString DataBase::outFullName(const int& idUser){ //Вывод записи приветсвия
    QSqlQuery fullName;
    fullName.prepare("SELECT * FROM authorization WHERE id = :idUser");
    fullName.bindValue(":idUser", idUser);
    if(fullName.exec()){
        while(fullName.next()){
            QStringList fullN = fullName.value(3).toString().split(" ");
            return "Здраствуйте! " + fullN[0] + " " + fullN[1][0] + ".";
        }
    }
    return "";
}

QString DataBase::outInterfaceObject(const int& idUser, int& dean){ //вывод интерфейса
    QSqlQuery interfaceObject;
    interfaceObject.prepare("SELECT * FROM authorization WHERE id = :idUser");
    interfaceObject.bindValue(":idUser", idUser);
    if(interfaceObject.exec()){
        if(interfaceObject.next()){
            dean = interfaceObject.value(4).toInt();
            if(dean == 1){
                return "Вы из деканата";
            }
            else{
                interfaceObject.prepare("SELECT * FROM object WHERE id = :idObject");
                interfaceObject.bindValue(":idObject", dean);
                interfaceObject.exec();
                if(interfaceObject.next()){
                    return "Вы преподователь по предмету: " + interfaceObject.value(1).toString();
                }
            }
        }
    }
    return "";
}

bool DataBase::checkFullName(QString& fullName){ //процесс редактирования и проверки имени
    QString timeFullName;
    int count = 1;
    for(int x = 0; x < fullName.size(); x++){
        if(fullName[x] != ' ' and x == fullName.size() - 1){
            timeFullName += fullName[x];
        }
        else if(fullName[x] == ' ' and x == fullName.size() - 1){
            continue;
        }
        else if(fullName[x] != ' '){
            timeFullName += fullName[x];
        }
        else if(fullName[x] != ' ' and fullName[x + 1] == ' '){
            timeFullName = timeFullName + fullName[x] + fullName[x + 1];
        }
        else if(fullName[x] == ' ' and fullName[x + 1] != ' '){
            count++;
            timeFullName += fullName[x];
            fullName[x+1] = fullName[x+1].toUpper();
        }
    }
    if(timeFullName.size() >= 3 and count == 2){
        timeFullName[0] = timeFullName[0].toUpper();
        fullName = timeFullName;
        return 0;
    }
    return 1;

}

bool DataBase::login(const QString& username, const QString& password, int& idUser){ //авторизация сотрудника учебного заведения
    QSqlQuery author;
    if(author.exec("SELECT * FROM authorization WHERE username = '"+username+"' AND pass = '"+password+"'")){
        int count = 0;
        while(author.next()){
            idUser = author.value(0).toInt();
            count++;
            if(author.value(5).toInt() == 1){
                count++;
            }
        }
        if (count == 1){
            return true;
        }
    }
    return false;
}

int DataBase::registr(const QString& username, const QString& password, const QString& nameObject, QString fullName, const int& access){ //добавление доступа
    if(username.size() < 1 or password.size() < 1 or checkFullName(fullName)){
        return 3;
        qDebug("Некоректные поля!");
    }
    QSqlQuery registr;

    if(registr.exec("SELECT * FROM authorization WHERE username = '"+username+"' AND access = 0")){ // 0 - успешно; 1 - повтор пользователя; 2 - разблокировка пользователя; 3 - отпровление некорректных полей
        while(registr.next()){
            qDebug("Есть такой пользователь!");
            return 1;
        }
    }
    if (access == 0 and registr.exec("SELECT * FROM authorization WHERE username = '"+username+"' AND access = 1")){
        while(registr.next()){
            qDebug("Пользователь заблокирован!");
            return 2;
        }
    }
    int idObject = 0;
    registr.exec("SELECT * FROM object WHERE name_object = '"+nameObject+"'");
    if(registr.next()){
        idObject = registr.value(0).toInt();
    }
    if(access == 1 and registr.exec("SELECT * FROM authorization WHERE username = '"+username+"' AND access = 1")){
        while(registr.next()){
            registr.prepare(("UPDATE authorization SET pass = :password, full_name = :full_name, id_object = :id_object,  access = 0, reasAccess = NULL WHERE username = '"+username+"'"));
            registr.bindValue(":password", password);
            registr.bindValue(":id_object", idObject);
            registr.bindValue(":full_name", fullName);
            registr.exec();
        }
        return 0;
    }

    registr.prepare("INSERT INTO authorization (username, pass, full_name, id_object, access) " "VALUES (:username, :password, :full_name, :id_object, 0)");
    registr.bindValue(":username", username);
    registr.bindValue(":password", password);
    registr.bindValue(":id_object", idObject);
    registr.bindValue(":full_name", fullName);
    registr.bindValue(":access", 0);
    registr.exec();
    return 0;
}

QString DataBase::cause(const QString& username){ //получение информации о блокировке
    QSqlQuery registr;
    if (registr.exec("SELECT * FROM authorization WHERE username = '"+username+"'")){
        while(registr.next()){
            return "Пользователь заблокирован по причине: " + registr.value(6).toString() + ".\nПродолжить?";
        }
    }
    return "";
}

bool DataBase::takeAccess(const QString& username, const QString& reasAccess){ //конфискация доступа
    QSqlQuery registr;
    if(registr.exec("SELECT * FROM authorization WHERE username = '"+username+"' AND access = 0")){
        if(registr.next()){
            registr.exec("UPDATE authorization SET access = 1, reasAccess = '"+reasAccess+"' WHERE username = '"+username+"'");
            return true;
        }
    }
    return false;
}


bool DataBase::checkFaculty(QString nameFaculty, bool addRemove){ //проверка наличия факультета
    if(nameFaculty.isEmpty()){
        return false;
    }
    else{
        nameFaculty[0] = nameFaculty[0].toUpper();
    }
    QSqlQuery registr;
    if(registr.exec("SELECT * FROM groups WHERE name = '"+nameFaculty+"'")){
        if(registr.next()){
            if(registr.exec("SELECT * FROM groups WHERE name = '"+nameFaculty+"' AND id_groups IS NULL")){
                if(registr.next()){
                    return (-1 + addRemove);
                }
                else{
                    return (0 * addRemove);
                }
            }
        }
        else{
            return (0 + addRemove);
        }
    }
    return false;
}

void DataBase::addFaculty(QString nameFaculty){
    QSqlQuery add;
    add.exec("INSERT INTO groups (name) " "VALUES ('"+nameFaculty+"')");
    checkFac();
}

void DataBase::removeFaculty(QString nameFaculty){
    QSqlQuery remove;
    remove.exec("DELETE FROM groups WHERE name = '"+nameFaculty+"'");
    checkFac();
}


bool DataBase::addObject(QString nameObject, bool credit){
    if(nameObject.isEmpty()){
        return false;
    }
    else{
        nameObject[0] = nameObject[0].toUpper();
    }
    QSqlQuery add;
    if(add.exec("SELECT * FROM object WHERE name_object = '"+nameObject+"'")){
        if(add.next()){
            return false;
        }
        else{
            add.prepare("INSERT INTO object (name_object, credit) " "VALUES ('"+nameObject+"', :credit)");
            add.bindValue(":credit", credit);
            add.exec();
            return true;
        }
    }
    return false;
}

QString DataBase::removeQuaObject(const QString& nameObject){
    QSqlQuery remove;
    int count = 0;
    if(remove.exec("SELECT * FROM authorization JOIN object ON authorization.id_object = object.id WHERE name_object = '"+nameObject+"'")){
        while(remove.next()){
            count++;
        }
    }
    if(remove.exec("SELECT * FROM object WHERE name_object = '"+nameObject+"'")){
        if(remove.next()){
            return ("Будут каскадно удаленны " + QString::number(count) + " (Количество преподователей)\n" + "Вы уверены?");
        }
        else{
            return "";
        }
    }
    return "";
}

void DataBase::removeObject(const QString& nameObject){
    QSqlQuery remove;
    remove.exec("DELETE FROM object WHERE name_object = '"+nameObject+"'");
}

bool DataBase::renameObject(const QString& nameObject, QString newNameObject){
    if(newNameObject.isEmpty()){
        return false;
    }
    else{
        newNameObject[0] = newNameObject[0].toUpper();
    }
    QSqlQuery rename;
    if(rename.exec("SELECT * FROM object WHERE name_object = '"+nameObject+"'")){
        if(rename.next()){
            rename.exec("UPDATE object SET name_object = '"+newNameObject+"' WHERE name_object = '"+nameObject+"'");
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void DataBase::listObject(const QString& nameObject, QTableView* tableViewObject){
    delete tableViewObject->model();
    QSqlQueryModel* list = new QSqlQueryModel();
    list->setQuery("SELECT username, full_name FROM authorization JOIN object ON authorization.id_object = object.id WHERE name_object = '"+nameObject+"' AND access = 0");
    list->setHeaderData(0, Qt::Horizontal, "Логин");
    list->setHeaderData(1, Qt::Horizontal, "Фамилия Имя");
    tableViewObject->setModel(list);
}

bool DataBase::removeStudent(const QString& student, const QString& oldGro, const QString& newGro){
    QSqlQuery remove;
    QStringList strList = student.split(" ");
    if(strList.size() >= 2 and !(oldGro == newGro)){
        remove.exec("SELECT student.id, groups.id FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+strList[0]+"' AND student.name = '"+strList[1]+"' AND groups.name = '"+oldGro+"'");
        if(remove.next()){
            int id_student = remove.value(0).toInt();
            int id_oldGro = remove.value(1).toInt();
            remove.exec("SELECT groups.id FROM groups WHERE groups.name = '"+newGro+"'");
            if(remove.next()){
                int id_newGro = remove.value(0).toInt();
                //std::vector<int> delIns;
                QSqlQuery cmd;
                remove.exec("SELECT id_groups, id_object FROM (SELECT *, COUNT(*) as ob_count FROM groupsObject JOIN groups ON groups.id = groupsObject.id_groups WHERE groups.name = '"+oldGro+"' or groups.name = '"+newGro+"' GROUP BY id_object) WHERE ob_count = 1");
                while (remove.next()) {
                    int id_obj = remove.value(1).toInt();
                    if(remove.value(0).toInt() == id_oldGro){
                        /*
                        delIns.push_back(0);
                        delIns.push_back(remove.value(1).toInt());
*/
                        cmd.prepare("DELETE FROM grades WHERE id_student = :id_student AND id_object = :id_obj");
                        cmd.bindValue(":id_student", id_student);
                        cmd.bindValue(":id_obj", id_obj);
                        cmd.exec();
                    }
                    else{
                        /*
                        delIns.push_back(1);
                        delIns.push_back(remove.value(1).toInt());
*/

                        cmd.prepare("INSERT INTO grades (id_student, id_object) " "VALUES (:id_student, :id_obj)");
                        cmd.bindValue(":id_student", id_student);
                        cmd.bindValue(":id_obj", id_obj);
                        cmd.exec();

                    }
                }
                remove.prepare("UPDATE student SET id_groups = :id_newGro WHERE id = :id_student");
                remove.bindValue(":id_newGro", id_newGro);
                remove.bindValue(":id_student", id_student);
                remove.exec();
                checkGru();
                return true;
            }
        }
    }
    return false;
}

bool DataBase::delStudent(const QString& student, const QString& gro){
    QSqlQuery del;
    QStringList strList = student.split(" ");
    int id_student = 0;
    bool count = false;
    del.exec("SELECT student.id FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+strList[0]+"' AND student.name = '"+strList[1]+"' AND groups.name = '"+gro+"'");
    while(del.next()){
        if (count) return false;
        count = true;
        id_student = del.value(0).toInt();
    }
    del.prepare("DELETE FROM student WHERE id = :id_student");
    del.bindValue(":id_student", id_student);
    del.exec();
    checkGru();
    return true;
}

int DataBase::chekAct(const QString& student, const QString& gro){
    QSqlQuery chek;
    QStringList strList = student.split(" ");
    if(strList.size() < 2) return false;
    chek.exec("SELECT active FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+strList[0]+"' AND student.name = '"+strList[1]+"' AND groups.name = '"+gro+"'");
    if(chek.next()){
        if(chek.value(0).toBool()){
            return 1;
        }
        else{
            return 2;
        }
    }
    return 0;
}

bool DataBase::addActStudent(const QString& student, const QString& gro){
    QSqlQuery addAct;
    QStringList strList = student.split(" ");
    int id_student = 0;
    bool count = false;
    addAct.exec("SELECT student.id FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+strList[0]+"' AND student.name = '"+strList[1]+"' AND groups.name = '"+gro+"'");
    while(addAct.next()){
        if (count) return false;
        count = true;
        id_student = addAct.value(0).toInt();
    }
    addAct.prepare("UPDATE student SET active = 1 WHERE id = :id_student");
    addAct.bindValue(":id_student", id_student);
    addAct.exec();
    //checkGru();
    return true;
}

bool DataBase::canActStudent(const QString& student, const QString& gro){
    QSqlQuery canAct;
    QStringList strList = student.split(" ");
    int id_student = 0;
    bool count = false;
    canAct.exec("SELECT student.id FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+strList[0]+"' AND student.name = '"+strList[1]+"' AND groups.name = '"+gro+"'");
    while(canAct.next()){
        if (count) return false;
        count = true;
        id_student = canAct.value(0).toInt();
    }
    canAct.prepare("UPDATE student SET active = 0 WHERE id = :id_student");
    canAct.bindValue(":id_student", id_student);
    canAct.exec();
    return true;
}

bool DataBase::addStudent(const QString& student_surname, const QString& student_name, const QString& student_patronymic, const QString& gro){
    QSqlQuery add;
    add.exec("SELECT student.id FROM student JOIN groups ON groups.id = student.id_groups WHERE surname = '"+student_surname+"' AND student.name = '"+student_name+"' AND groups.name = '"+gro+"'");
    if(add.next()){
        return 0;
    }
    add.exec("SELECT id FROM groups WHERE name = '"+gro+"'");
    if(add.next()){
        QSqlQuery addIns;
        int grId = add.value(0).toInt();
        addIns.prepare("INSERT INTO student (surname, name, patronymic, id_groups) " "VALUES ('"+student_surname+"', '"+student_name+"', '"+student_patronymic+"', :idGroups)");
        addIns.bindValue(":idGroups", grId);
        addIns.exec();
        checkGru();
        return true;
    }
    return false;
}

bool DataBase::renameGro(const QString& oldName, const QString& newName){
    QSqlQuery rename;
    rename.exec("SELECT * from groups where name = '"+newName+"'");
    if(rename.next()) return 0;
    rename.exec("UPDATE groups SET name = '"+newName+"' WHERE name = '"+oldName+"'");
    checkGru();
    checkFac();
    return 1;
}

bool DataBase::addGro(const QString& nameFac, const QString& nameGro){
    QSqlQuery add;
    add.exec("SELECT * from groups where name = '"+nameGro+"'");
    if(add.next()) return 0;
    int idFac;
    add.exec("SELECT * from groups where name = '"+nameFac+"'");
    if(add.next()) idFac = add.value(0).toInt();
    else return 0;
    add.prepare("INSERT INTO groups (name, id_groups) " "VALUES ('"+nameGro+"', :idFac)");
    add.bindValue(":idFac", idFac);
    add.exec();
    checkGru();
    checkFac();
    return 1;
}

bool DataBase::addSubGro(const QString& nameGro, const QString& nameSub){
    QSqlQuery addSub;
    int idGr, idSub;
    addSub.exec("SELECT id from groups where name = '"+nameGro+"'");
    if(addSub.next()) idGr = addSub.value(0).toInt();
    else return 0;
    addSub.exec("SELECT id from object where name_object = '"+nameSub+"'");
    if(addSub.next()){
        idSub = addSub.value(0).toInt();
        if(idSub == 1) return 0;
    }
    else return 0;
    addSub.prepare("SELECT * from groupsObject where id_groups = :idGroups AND id_object = :idObject");
    addSub.bindValue(":idGroups", idGr);
    addSub.bindValue(":idObject", idSub);
    addSub.exec();
    if(addSub.next()) return 0;
    addSub.prepare("INSERT INTO groupsObject (id_groups, id_object) " "VALUES (:idGroups, :idObject)");
    addSub.bindValue(":idGroups", idGr);
    addSub.bindValue(":idObject", idSub);
    addSub.exec();
    return 1;
}

bool DataBase::delSubGro(const QString& nameGro, const QString& nameSub){
    QSqlQuery delSub;
    int idGr, idSub;
    delSub.exec("SELECT id from groups where name = '"+nameGro+"'");
    if(delSub.next()) idGr = delSub.value(0).toInt();
    else return 0;
    delSub.exec("SELECT id from object where name_object = '"+nameSub+"'");
    if(delSub.next()) {
        idSub = delSub.value(0).toInt();
        if(idSub == 1) return 0;
    }
    else return 0;
    delSub.prepare("SELECT * from groupsObject where id_groups = :idGroups AND id_object = :idObject");
    delSub.bindValue(":idGroups", idGr);
    delSub.bindValue(":idObject", idSub);
    delSub.exec();
    if(!(delSub.next())) return 0;
    delSub.prepare("DELETE FROM groupsObject WHERE id_groups = :idGroups AND id_object = :idObject");
    delSub.bindValue(":idGroups", idGr);
    delSub.bindValue(":idObject", idSub);
    delSub.exec();
    return 1;
}

QString DataBase::removeQuaGroup(const QString& gr){
    QSqlQuery remove;
    int count = 0;
    if(remove.exec("SELECT * FROM student JOIN groups ON student.id_groups = groups.id WHERE groups.name = '"+gr+"'")){
        while(remove.next()){
            count++;
        }
    }
    if(remove.exec("SELECT * FROM groups WHERE groups.name = '"+gr+"'")){
        if(remove.next()){
            return ("Будут каскадно удаленны " + QString::number(count) + " (Количество студентов)\n" + "Вы уверены?");
        }
        else{
            return "";
        }
    }
    return "";
}

void DataBase::removeGroup(const QString& gr){
    QSqlQuery remove;
    remove.exec("DELETE FROM groups WHERE name = '"+gr+"'");
    checkGru();
    checkFac();
}

void DataBase::intTable(QTableView* tableStudent){
    this->tableStu = tableStudent;
}

void DataBase::clearTable(){
    tableStu->setModel(0);
    tableStu->setHorizontalHeader(0);
    tableStu->setVerticalHeader(0);
}

void DataBase::listStudent(int idDean, const QString& nameObject){
    clearTable();
    delete tableStu->model();
    //chekTable();
    if(idDean == 1){
        QSqlQuery listSt;
        listSt.exec("SELECT id FROM object WHERE name_object = '"+nameObject+"'");
        if(listSt.next()){
            idDean = listSt.value(0).toInt();
        }
    }
    //QSqlTableModel* list = new QSqlTableModel();
    QSqlRelationalTableModel* list = new QSqlRelationalTableModel();
    QString cmd = "SELECT student.surname, student.name, student.patronymic, groups.name, grades.lecture, grades.laboratory, grades.practice, grades.individual, grades.CGT, grades.tests, grades.exam, grades.gra FROM grades JOIN student ON grades.id_student = student.id JOIN groups ON student.id_groups = groups.id WHERE grades.id_object = " + QString::number(idDean);
    list->setQuery(cmd);
    list->setHeaderData(0, Qt::Horizontal, "Фамилия");
    list->setHeaderData(1, Qt::Horizontal, "Имя");
    list->setHeaderData(2, Qt::Horizontal, "Отчество");
    list->setHeaderData(3, Qt::Horizontal, "Группа");
    list->setHeaderData(4, Qt::Horizontal, "лекции");
    list->setHeaderData(5, Qt::Horizontal, "лабораторные");
    list->setHeaderData(6, Qt::Horizontal, "практики");
    list->setHeaderData(7, Qt::Horizontal, "индивидуальные");
    list->setHeaderData(8, Qt::Horizontal, "РГР");
    list->setHeaderData(9, Qt::Horizontal, "тесты");
    list->setHeaderData(10, Qt::Horizontal, "экзамен");
    list->setHeaderData(11, Qt::Horizontal, "Итоговый результат");
    tableStu->setModel(list);
}

QString DataBase::chekTable(int idDean, const QString& nameObject){
    std::vector<int> error;
    std::vector<QString> grades = {"lecture", "laboratory", "practice", "individual", "CGT", "tests", "exam"};
    if(tableStu->model() == nullptr){
        return "Ошибка";
    }
    for(int row = 0; row < tableStu->model()->rowCount(); row++){
        int sumGrad = 0;
        int qua = 0;
        QString surname = tableStu->model()->data(tableStu->model()->index(row, 0)).toString();
        QString name = tableStu->model()->data(tableStu->model()->index(row, 1)).toString();
        QString groups = tableStu->model()->data(tableStu->model()->index(row, 3)).toString();
        QSqlQuery upGrad;
        for(int column = 4; column < tableStu->model()->columnCount() - 1; column++){
            if(idDean == 1){
                QSqlQuery listSt;
                listSt.exec("SELECT id FROM object WHERE name_object = '"+nameObject+"'");
                if(listSt.next()){
                    idDean = listSt.value(0).toInt();
                }
            }
            QString grad = tableStu->model()->data(tableStu->model()->index(row, column)).toString();
            if(grad == ""){
                QString cmd = ("UPDATE grades SET "+grades[column - 4]+" = NULL WHERE id_student = (SELECT student.id FROM student JOIN groups ON student.id_groups = groups.id WHERE student.surname = '"+surname+"' AND student.name = '"+name+"' AND groups.name = '"+groups+"') AND id_object = "+QString::number(idDean));
                upGrad.exec(cmd);
            }
            else{
                int gradInt = grad.toInt();
                if(gradInt >= 0 and gradInt <= 100){
                    QString cmd = ("UPDATE grades SET "+grades[column - 4]+" = "+QString::number(gradInt)+" WHERE id_student = (SELECT student.id FROM student JOIN groups ON student.id_groups = groups.id WHERE student.surname = '"+surname+"' AND student.name = '"+name+"' AND groups.name = '"+groups+"') AND id_object = "+QString::number(idDean));
                    upGrad.exec(cmd);
                    sumGrad += gradInt;
                    qua++;
                }
                else{
                    error.push_back(row);
                    error.push_back(column);
                }
            }
        }
        if(qua > 0){
            double sum = (double)(((int)(((double)sumGrad/(double)qua) * 100)) / 100.0);
            QString cmd = ("UPDATE grades SET gra = "+QString::number(sum, 'g', 4)+" WHERE id_student = (SELECT student.id FROM student JOIN groups ON student.id_groups = groups.id WHERE student.surname = '"+surname+"' AND student.name = '"+name+"' AND groups.name = '"+groups+"') AND id_object = "+QString::number(idDean));
            upGrad.exec(cmd);
        }
        else{
            QString cmd = ("UPDATE grades SET gra = NULL WHERE id_student = (SELECT student.id FROM student JOIN groups ON student.id_groups = groups.id WHERE student.surname = '"+surname+"' AND student.name = '"+name+"' AND groups.name = '"+groups+"') AND id_object = "+QString::number(idDean));
            upGrad.exec(cmd);
        }
    }
    listStudent(idDean, nameObject);
    if(error.size() > 0){
        QString errorInfo = "";
        for(size_t x = 0; x < error.size(); x+=2){
            errorInfo += "Ошибка в записи по строке " + QString::number(error[x] + 1) + ", и в столбце " + QString::number(error[x+1] + 1) + "\n";
        }
        return errorInfo;
    }
    return "Все выполнилось успешно";
}

void DataBase::tabDeb(QTableView* tableStudent, const QString &grName, double price){
    delete tableStudent->model();
    QSqlQuery tabSQL;
    int old_id = -100;
    bool chek = false;
    double pr;
    bool act;
    bool check3 = false;
    QString cause = "";
    std::vector<Student> tabStu;
    Student student;
    tabSQL.exec("SELECT student.id, student.active, credit, gra, groups.name, CONCAT(student.surname, ' ', student.name, ' ', student.patronymic) FROM grades JOIN student ON grades.id_student = student.id JOIN groups ON student.id_groups = groups.id JOIN object ON grades.id_object = object.id AND groups.name = '"+grName+"' GROUP BY student.id");
    while(tabSQL.next()){
        int id_stu = tabSQL.value(0).toInt();
        if(id_stu != old_id){
            old_id = id_stu;
            if(chek){
                student.pr = pr;
                if(pr > 1) cause+=", учится на 5.";
                else if(check3) cause+=", учится на 3.";
                else cause+=", учится на 4 и/или 5.";
                student.cause = cause;
                tabStu.push_back(student);
                check3 = false;
            }
            act = tabSQL.value(1).toBool();
            if(act){
                pr = 1.5;
                cause = "Активный студент";
            }
            else{
                pr = 1.25;
                cause = "Неактивный студент";
            }
            student.name_gr = tabSQL.value(4).toString();
            student.full_name = tabSQL.value(5).toString();
            chek = true;
        }
        if(chek){
            if(tabSQL.value(3).toInt() < 50){
                chek = false;
            }
            else if(!(tabSQL.value(2).toBool()) and (tabSQL.value(3).toInt() >= 50) and (tabSQL.value(3).toInt() <= 72)){
                if(act){
                    pr = 1;
                    act = false;
                }
                else{
                    chek = false;
                }
            }
            else if(!(tabSQL.value(2).toBool()) and (tabSQL.value(3).toInt() >= 73) and (tabSQL.value(3).toInt() <= 86)){
                pr = 1;
            }
        }
    }
    if(chek){
        student.pr = pr;
        if(pr > 1) cause+=", учится на 5.";
        else if(check3) cause+=", учится на 3.";
        else cause+=", учится на 4 и/или 5.";
        student.cause = cause;
        tabStu.push_back(student);
    }
    QStandardItemModel *listDeb = new QStandardItemModel(tabStu.size(), 4);
    listDeb->setHeaderData(0, Qt::Horizontal, "Стипендия");
    listDeb->setHeaderData(1, Qt::Horizontal, "Группа");
    listDeb->setHeaderData(2, Qt::Horizontal, "ФИО");
    listDeb->setHeaderData(3, Qt::Horizontal, "Причина стипендии");
    for(size_t i = 0; i < tabStu.size(); i ++){
        listDeb->setItem(i, 0, new QStandardItem(QString::number(tabStu[i].pr * price) + "\u20BD"));
        listDeb->setItem(i, 1, new QStandardItem(tabStu[i].name_gr));
        listDeb->setItem(i, 2, new QStandardItem(tabStu[i].full_name));
        listDeb->setItem(i, 3, new QStandardItem(tabStu[i].cause + " Надбавка: " + QString::number(pr) + "."));
    }
    tableStudent->setModel(listDeb);
}
