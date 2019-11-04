#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include "gpio.hpp"
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <stdlib.h> //for using the function sleep
#include <unistd.h>
class MainApplication : public QObject {
  Q_OBJECT

  GPIO *gpio;
  unsigned int milliseconds = 1000; // 1 second
public:
  explicit MainApplication(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MAINAPPLICATION_H
