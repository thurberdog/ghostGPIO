#include "mainapplication.h"

MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
  setGPIO(0, 0);
  sleep(3);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
  setGPIO(0, 1);
  sleep(3);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
  setGPIO(1, 0);
  sleep(3);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
  setGPIO(1, 1);
  sleep(3);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
}

QString MainApplication::getGPIO() {
  QString pins;
  QString command = "i2cget";
  QStringList arguments;
  i2cget.setProgram(command);
  arguments << "-y";
  arguments << "0";
  arguments << "0x3E";
  arguments << "0";
  i2cget.setArguments(arguments);
  i2cget.start();
  i2cget.waitForReadyRead();
  pins = i2cget.readAllStandardOutput();
  return pins;
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
  arguments << "-y -r 0 0x3E 1 ";
  arguments << "-r";
  arguments << "0";
  arguments << "0x3E";
  arguments << "1";
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
  qDebug() << __LINE__ << __FUNCTION__ << "i2cset started " << pin3 << ","
           << pin4;
  i2cset.waitForReadyRead(-1);
  QString response = i2cset.readAllStandardOutput();
  qDebug() << __LINE__ << __FUNCTION__ << response;
  i2cset.waitForFinished(-1);
}
