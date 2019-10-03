#include "mainapplication.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainApplication mainApp;
    return a.exec();
}
