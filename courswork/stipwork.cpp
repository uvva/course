#include "stipwork.h"
#include "ui_stipwork.h"

stipWork::stipWork(DataBase *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::stipWork)
{
    ui->setupUi(this);
    this->db = db;
    db->inComboBoxFac(ui->listFac);
}

stipWork::~stipWork()
{
    delete ui;
}

void stipWork::on_back_clicked()
{
    this->close();
    emit stip();
}

void stipWork::on_pushStudent_clicked()
{
    double price = ui->addStep->text().toDouble();
    if(ui->listGr->currentText() == "") return;
    db->tabDeb(ui->tableName_2, ui->listGr->currentText(), price);
}

void stipWork::on_print_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableName_2->model()->rowCount();
    const int columnCount = ui->tableName_2->model()->columnCount();

    out <<  "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        <<  QString("<title>%1</title>\n").arg("Стипендия")
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<h2>Для "+ui->listGr->currentText()+" из "+ui->listFac->currentText()+" назначить к стипендии следующих студентов по таблице, на срок(01.09.2024 - 01.01.2025):</h2>"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->tableName_2->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tableName_2->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tableName_2->isColumnHidden(column)) {
                QString data = ui->tableName_2->model()->data(ui->tableName_2->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
           "</body>\n"
           "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;
    document->print(&printer);
    delete document;
}

void stipWork::on_listFac_currentTextChanged(const QString &arg)
{
    db->checkGruForApp(arg, ui->listGr);
}
