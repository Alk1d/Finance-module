#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "md5.h"
namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_reg_cancel_button_clicked();

    void on_reg_ok_button_clicked();

private:
    Ui::Register *ui;
    MD5 *reg_md5;
};

#endif // REGISTER_H
