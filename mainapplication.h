#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include "gpio.hpp"
#include <unistd.h>
#include <stdlib.h>     //for using the function sleep
#include <QObject>
#include <QDebug>
#include <QProcess>
class MainApplication : public QObject
{
    Q_OBJECT

    GPIO *gpio;

public:
    explicit MainApplication(QObject *parent = nullptr);

signals:

public slots:

};

#endif // MAINAPPLICATION_H
