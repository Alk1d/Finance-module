#include "dataadd.h"
#include "ui_dataadd.h"
#include <QSqlQuery>
#include <QMessageBox>
DataAdd::DataAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataAdd)
{
    ui->setupUi(this);
    data_md5 = new MD5 ();
    category_comboboxModel = new QSqlQueryModel(ui->category_comboBox);
    category_combobox_setQuery();

    ui->add_dateEdit->setDate(QDate::currentDate());
}

DataAdd::~DataAdd()
{
    delete ui;
}




void DataAdd::on_datacancel_button_clicked()
{
    ui->category_lineEdit->clear();
    ui->value_lineEdit->clear();
    hide();
}


void DataAdd::on_dataadd_button_clicked()
{
    DB_CategoryID = ui->category_comboBox->currentIndex();
    if (ui->table_comboBox->currentText() == "Income")
    {
    DB_IncomeID = 0;
    QSqlQuery income_IncomeID_query(QSqlDatabase::database("personal_finance"));
    income_IncomeID_query.prepare(QString("SELECT max (\"IncomeID\") FROM \"Income\""));
    income_IncomeID_query.exec();
    if(income_IncomeID_query.next()) DB_IncomeID = income_IncomeID_query.value(0).toInt();

    QSqlQuery income_add_query(QSqlDatabase::database("personal_finance"));
    income_add_query.prepare(QString("insert into \"Income\" values (:IncomeID, :Income_Date, :CategoryID, :Income_Value, :UserID)"));
    income_add_query.bindValue(":IncomeID", ++DB_IncomeID);
    income_add_query.bindValue(":Income_Date", ui->add_dateEdit->date());
    income_add_query.bindValue(":CategoryID", ++DB_CategoryID);
    income_add_query.bindValue(":Income_Value", ui->value_lineEdit->text().toInt());
    income_add_query.bindValue(":UserID", data_userID);

    if (income_add_query.exec())
    {
        ui->value_lineEdit->clear();
        QMessageBox::information(this,"Success","Data added");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Wrong input data!");
    }
    }
    else
    {
        DB_ExpenseID = 0;
        QSqlQuery expense_ExpenseID_query(QSqlDatabase::database("personal_finance"));
        expense_ExpenseID_query.prepare(QString("SELECT max (\"ExpenseID\") FROM \"Expense\""));
        expense_ExpenseID_query.exec();
        if(expense_ExpenseID_query.next()) DB_ExpenseID = expense_ExpenseID_query.value(0).toInt();

        QSqlQuery expense_add_query(QSqlDatabase::database("personal_finance"));
        expense_add_query.prepare(QString("insert into \"Expense\" values (:ExpenseID, :Expense_Date, :CategoryID, :Expense_Value, :UserID)"));
        expense_add_query.bindValue(":ExpenseID", ++DB_ExpenseID);
        expense_add_query.bindValue(":Expense_Date", ui->add_dateEdit->date());
        expense_add_query.bindValue(":CategoryID", ++DB_CategoryID);
        expense_add_query.bindValue(":Expense_Value", ui->value_lineEdit->text().toInt());
        expense_add_query.bindValue(":UserID", data_userID);

        if (expense_add_query.exec())
        {
            ui->value_lineEdit->clear();
            QMessageBox::information(this,"Success","Data added");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Wrong input data!");
        }
    }
}

void DataAdd::setData_userID(int newData_userID)
{
    data_userID = newData_userID;
}

void DataAdd::category_combobox_setQuery()
{
    category_comboboxModel->setQuery("SELECT \"CategoryName\" FROM \"Category\"");
    ui->category_comboBox->setModel(category_comboboxModel);
}

void DataAdd::on_addcategory_button_clicked()
{
    DB_CategoryID = 0;
    QSqlQuery category_CategoryID_query(QSqlDatabase::database("personal_finance"));
    category_CategoryID_query.prepare(QString("SELECT max (\"CategoryID\") FROM \"Category\""));
    category_CategoryID_query.exec();
    if(category_CategoryID_query.next()) DB_CategoryID = category_CategoryID_query.value(0).toInt();

    QSqlQuery category_add_query(QSqlDatabase::database("personal_finance"));
    category_add_query.prepare(QString("insert into \"Category\" values(:CategoryID, :CategoryName)"));
    category_add_query.bindValue(":CategoryID", ++DB_CategoryID);
    category_add_query.bindValue(":CategoryName", ui->category_lineEdit->text());
    if (category_add_query.exec())
    {
        ui->category_lineEdit->clear();
        QMessageBox::information(this,"Success","Category added");
        category_combobox_setQuery();
    }
    else
    {
        ui->category_lineEdit->clear();
        QMessageBox::warning(this, "Error", "Wrong input data or category already exsists!");
    }
}

