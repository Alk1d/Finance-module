#include "modulewindow.h"
#include "dataadd.h"
#include "md5.h"
#include "qdir.h"
#include "qsqlerror.h"
#include "qsqlrelationaldelegate.h"
#include "qsqlrelationaltablemodel.h"
#include "ui_modulewindow.h"
#include <QMessageBox>
#include "QDate"
#include <QSortFilterProxyModel>
#include <QFile>
ModuleWindow::ModuleWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ModuleWindow)
{

    ui->setupUi(this);
    connect(ui->actionEditable_tables_data, SIGNAL(triggered()), this, SLOT(slot_MenuChecked()));
    mod_md5 = new MD5 ();

    QString path("Finance_module_v3");
    QDir dir;

    if (!dir.exists(path))
        dir.mkpath(path);

    QFile file("db_connection.txt");
    QString dbPath = "db_connection.txt";
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        fields += line.split(",");
    }

    db = QSqlDatabase::addDatabase(fields.at(0));
    db.setDatabaseName(fields.at(1));
    db.setUserName(fields.at(2));
    db.setPassword(fields.at(3));
    if (!db.open())
    {
        QMessageBox::warning(0, "Error connecting to Database", db.lastError().text());
    }
    else
    {
        QMessageBox::information(0, "Connection", "Connection Success");
    }

    ui->income_tableView->setItemDelegate(new QSqlRelationalDelegate(ui->income_tableView));
    ui->expense_tableView->setItemDelegate(new QSqlRelationalDelegate(ui->expense_tableView));
    ui->income_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->expense_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    Data_window = new DataAdd (this);

    file.close();
    }
ModuleWindow::~ModuleWindow()
{
    delete ui;
}

void ModuleWindow::showEvent(QShowEvent*)
{
    income_model = new QSqlRelationalTableModel (this, db);
    income_model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    income_model->setTable("\"Income\"");
    ui->income_tableView->setColumnHidden(0, true);
    ui->income_tableView->setColumnHidden(4, true);

    income_model->setHeaderData(1, Qt::Horizontal, tr("Date"));
    income_model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    income_model->setHeaderData(3, Qt::Horizontal, tr("Value"));

    income_model->setRelation(2, QSqlRelation("\"Category\"", "\"CategoryID\"", "\"CategoryName\""));
    income_model->select();
    if (!income_model->select())
        {
            QSqlError err = income_model->lastError();
            QMessageBox::information(0,
                                     qApp->tr("Failed to select data from table"),
                                     err.text(),
                                     QMessageBox::Ok);
        }


    expense_model = new QSqlRelationalTableModel (this, db);
    expense_model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    expense_model->setTable("\"Expense\"");
    expense_model->setHeaderData(1, Qt::Horizontal, tr("Date"));
    expense_model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    expense_model->setHeaderData(3, Qt::Horizontal, tr("Value"));

    expense_model->setRelation(2, QSqlRelation("\"Category\"", "\"CategoryID\"", "\"CategoryName\""));
    expense_model->select();
    if (!expense_model->select())
        {
            QSqlError err = expense_model->lastError();
            QMessageBox::information(0,
                                     qApp->tr("Failed to select data from table"),
                                     err.text(),
                                     QMessageBox::Ok);
        }
    QDate current_month(QDate::currentDate());
    int m = current_month.month();
    int y = current_month.year();
    current_month.setDate(y, m ,1);
    ui->dateEditFrom->setDate(current_month);
    ui->dateEditTo->setDate(QDate::currentDate());

   date_sort();
}

void ModuleWindow::on_sort_button_clicked()
{
    date_sort();
}

void ModuleWindow::date_sort()
{
    income_range = 0;
    expense_range = 0;
    total_range = 0;

    income_model->setFilter(QString("\"Income_Date\" between '%1' and '%2' and \"UserID\" = %3").arg(ui->dateEditFrom->date().toString("yyyy-MM-dd"),
                                                                                                      ui->dateEditTo->date().toString("yyyy-MM-dd")).arg(mod_userID));
    ui->income_tableView->setModel(income_model);
    for (int i = 0; i < income_model->rowCount(); ++i)
    {
        income_range += income_model->data(income_model->index(i, 3)).toInt();
    }
    ui->income_tableView->setColumnHidden(0, true);
    ui->income_tableView->setColumnHidden(4, true);

    expense_model->setFilter(QString("\"Expense_Date\" between '%1' and '%2' and \"UserID\" = %3").arg(ui->dateEditFrom->date().toString("yyyy-MM-dd"),
                                                                                                       ui->dateEditTo->date().toString("yyyy-MM-dd")).arg(mod_userID));
    expense_model->select();
    ui->expense_tableView->setModel(expense_model);
    for (int i = 0; i < expense_model->rowCount(); ++i)
    {
        expense_range += expense_model->data(expense_model->index(i, 3)).toInt();
    }
    ui->expense_tableView->setColumnHidden(0, true);
    ui->expense_tableView->setColumnHidden(4, true);

    total_range = income_range - expense_range;
    ui->income_range_lineedit->setText(QString::number(income_range));
    ui->expense_range_lineedit->setText(QString::number(expense_range));
    ui->total_lineedit->setText(QString::number(total_range));
}

void ModuleWindow::on_add_data_button_clicked()
{
    Data_window->setData_userID(getMod_userID());
    Data_window->show();
}

void ModuleWindow::setMod_userID(int newMod_userID)
{
    mod_userID = newMod_userID;
}


void ModuleWindow::on_revert_button_clicked()
{
    db.rollback();
}

void ModuleWindow::on_income_deletebutton_clicked()
{
    int selected_incomeRow = ui->income_tableView->currentIndex().row();
        if (selected_incomeRow >= 0)
        {
            income_model->removeRow(selected_incomeRow);
            selected_incomeRow = -1;
            income_model->select();
        }
        else QMessageBox::information(0, "Income delete", "No selected rows!");
}


void ModuleWindow::on_expense_deletebutton_clicked()
{
    int selected_expenseRow = ui->expense_tableView->currentIndex().row();
        if (selected_expenseRow >= 0)
        {
            expense_model->removeRow(selected_expenseRow);
            selected_expenseRow = -1;
            expense_model->select();
        }
        else QMessageBox::information(0, "Expense delete", "No selected rows!");
}

void ModuleWindow::slot_MenuChecked()
{
    if (ui->actionEditable_tables_data->isChecked())
    {
        QMessageBox::warning(0, "Edit menu", "To make permanent data changes, press ENTER after changing data in cells!");
        ui->income_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
        ui->expense_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else
    {
        ui->income_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->expense_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

int ModuleWindow::getMod_userID() const
{
    return mod_userID;
}

