#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include "modulewindow.h"
#include "register.h"
#include "md5.h"
namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    int global_userID;
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();


private slots:
    void on_auth_cancel_button_clicked();

    void on_auth_ok_button_clicked();

    void on_auth_register_button_clicked();

private:
    Ui::Authentication *ui;
    Register *register_window;
    ModuleWindow *module_window;
    MD5 *auth_md5;

};

#endif // AUTHENTICATION_H
