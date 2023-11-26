#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
//#include <QCryptographicHash>

inline bool Connection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("personal_finance");
    db.setUserName("postgres");
    db.setPassword("247725"); // Need to remove password from code!
    if (!db.open())
    {
        QMessageBox::warning(0, "Ошибка подлючения к БД", db.lastError().text());
        return false;
    }
    else
    {
        QMessageBox::information(0, "Соединение", "Подключено к БД");
        return true;
    }
}
#endif // DB_CONNECTION_H
