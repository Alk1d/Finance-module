#include "md5.h"

MD5::MD5()
{

}

void MD5::MD5_converter(QString &string)
{
    QString md5_login;
    QByteArray str_login;
    str_login = QCryptographicHash::hash(string.toLatin1(),QCryptographicHash::Md5);
    string = md5_login.append(str_login.toHex());
}
