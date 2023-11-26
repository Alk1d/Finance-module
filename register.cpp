#include "register.h"
#include "ui_register.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include "md5.h"
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    reg_md5 = new MD5 ();
}

Register::~Register()
{
    delete ui;
}

void Register::on_reg_cancel_button_clicked()
{
    ui->reg_username_lineEdit->clear();
    ui->reg_password_lineEdit->clear();
    hide();
}


void Register::on_reg_ok_button_clicked()
{
    QString reg_username = ui->reg_username_lineEdit->text();
    QString reg_password = ui->reg_password_lineEdit->text();

    reg_md5->MD5_converter(reg_username);
    reg_md5->MD5_converter(reg_password);

    QSqlQuery userid_query(QSqlDatabase::database("personal_finance"));
    int row_count = 0;
    userid_query.prepare(QString("select max (\"UserID\") from \"User\""));
    userid_query.exec();
    if(userid_query.next()) row_count = userid_query.value(0).toInt();

    QSqlQuery reg_query(QSqlDatabase::database("personal_finance"));
    reg_query.prepare(QString("insert into \"User\" values (:userID, :username, :password)"));
    reg_query.bindValue(":userID", ++row_count);
    reg_query.bindValue(":username", reg_username);
    reg_query.bindValue(":password", reg_password);
    if(reg_query.exec())
    {
        QMessageBox::information(this,"Success","Register succeccful");
        ui->reg_username_lineEdit->clear();
        ui->reg_password_lineEdit->clear();
        hide();
    }
    else
    {
        QSqlError err = reg_query.lastError().databaseText();
        QMessageBox::information(0,
                                 qApp->tr("Error! Repeated login"),
                                 err.text(),
                                 QMessageBox::Ok);
    }
}

