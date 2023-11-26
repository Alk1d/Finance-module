#include "authentication.h"
#include "modulewindow.h"
#include "ui_authentication.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include "md5.h"
Authentication::Authentication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authentication)
{
    setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);
    auth_md5 = new MD5 ();
    module_window = new ModuleWindow (this);
    register_window = new Register (this);

}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::on_auth_cancel_button_clicked()
{
    close();
}


void Authentication::on_auth_ok_button_clicked()
{

    QString auth_username = ui->auth_username_lineEdit->text();
    QString auth_password = ui->auth_username_lineEdit->text();

    auth_md5->MD5_converter(auth_username);
    auth_md5->MD5_converter(auth_password);

    QSqlQuery auth_query(QSqlDatabase::database("personal_finance"));
    auth_query.prepare(QString("select \"UserID\" from \"User\" where \"Login\" = :username and \"Password\" = :password"));
    auth_query.bindValue(":username", auth_username);
    auth_query.bindValue(":password", auth_password);
    auth_query.exec();
    if(auth_query.next())
    {
        QMessageBox::information(this,"Success","Logging in");
        global_userID = auth_query.value(0).toInt();
        module_window->setMod_userID(global_userID);
        module_window->show();
    }

    else
    {
        QMessageBox::information(this,"Error","Login or password are incorrect!");
    }
}

void Authentication::on_auth_register_button_clicked()
{

    register_window->show();
}




