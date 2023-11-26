#ifndef MODULEWINDOW_H
#define MODULEWINDOW_H

#include "dataadd.h"
#include "md5.h"
#include "qsortfilterproxymodel.h"
#include "qsqldatabase.h"
#include "qsqlrelationaltablemodel.h"
#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
QT_BEGIN_NAMESPACE
namespace Ui { class ModuleWindow; }
QT_END_NAMESPACE

class ModuleWindow : public QMainWindow
{
    Q_OBJECT

public:
    ModuleWindow(QWidget *parent = nullptr);
    ~ModuleWindow();

    void setMod_userID(int newMod_userID);

    int getMod_userID() const;

private slots:
    virtual void showEvent(QShowEvent*);

    void on_sort_button_clicked();

    void date_sort();

    void on_add_data_button_clicked();

    void on_revert_button_clicked();

    void on_income_deletebutton_clicked();

    void on_expense_deletebutton_clicked();

    void slot_MenuChecked();

private:
    int income_range = 0;
    int expense_range = 0;
    int total_range = 0;
    int mod_userID;

    Ui::ModuleWindow *ui;

    QSqlDatabase db;
    QSqlRelationalTableModel *income_model;
    QSqlRelationalTableModel *expense_model;
    DataAdd *Data_window;
    MD5 *mod_md5;
    QStringList fields;
};
#endif // MODULEWINDOW_H
