#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include "gpio.hpp"
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <stdlib.h> //for using the function sleep
#include <unistd.h>
class MainApplication : public QObject {
  Q_OBJECT

  GPIO *gpio;
  int milliseconds = 1000; // 1 second
public:
  explicit MainApplication(QObject *parent = nullptr);
  QTimer *timer;
  int leds = 0;
signals:

public slots:
  void update();
};

#endif // MAINAPPLICATION_H
