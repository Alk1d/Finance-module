#include "authentication.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Authentication x;
    x.show();
    return a.exec();
}
