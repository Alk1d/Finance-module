#ifndef DATAADD_H
#define DATAADD_H

#include "md5.h"
#include "qsqlquerymodel.h"
#include <QDialog>

namespace Ui {
class DataAdd;
}

class DataAdd : public QDialog
{
    Q_OBJECT

public:
    explicit DataAdd(QWidget *parent = nullptr);
    ~DataAdd();

    void setData_userID(int newData_userID);

private slots:

    void on_datacancel_button_clicked();

    void on_dataadd_button_clicked();

    void category_combobox_setQuery();

    void on_addcategory_button_clicked();

private:
    int data_userID;
    int DB_CategoryID;
    int DB_IncomeID;
    int DB_ExpenseID;
    QSqlQueryModel *category_comboboxModel;
    Ui::DataAdd *ui;
    MD5 *data_md5;
};

#endif // DATAADD_H
