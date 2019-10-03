#include "mainapplication.h"

MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
  setGPIO(0, 0);
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
  setGPIO(0, 1);
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
  setGPIO(1, 0);
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
  setGPIO(1, 1);
}

/**
 * @brief MainApplication::setGPIO
 * @param pin3
 * @param pin4
 */
void MainApplication::setGPIO(bool pin3, bool pin4) {
  // pin 0 & 1 input from the controller
  // pin 2 & 3 output to the controller
  QString command = "i2cset";
  QStringList arguments;
  arguments << "-y 0 0x3E 1 ";
  if ((pin3) && (pin4)) {
    arguments << "12";
  }
  if ((!pin3) && (pin4)) {
    arguments << "8";
  }
  if ((pin3) && (!pin4)) {
    arguments << "4";
  }
  if ((!pin3) && (!pin4)) {
    arguments << "0";
  }
  i2cset.setProgram(command);
  i2cset.setArguments(arguments);
  while (i2cset.state() == QProcess::NotRunning) {
    i2cset.start();
  }
  qDebug()<<__LINE__<<__FUNCTION__<<"i2cset started "<<pin3 << ","<<pin4;
  i2cset.waitForFinished();
}
