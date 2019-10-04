#include "mainapplication.h"

MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  gpioProcess = new  QProcess();
  connect(gpioProcess, SIGNAL(started()), this, SLOT(startedGPIO()));
  connect(gpioProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinish(int,QProcess::ExitStatus)));
  connect(gpioProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readGPIO()));
  connect(gpioProcess,SIGNAL(readyReadStandardError()),this,SLOT(readGPIOerror()));
connect( gpioProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(stateChanged(QProcess::ProcessState)) );

  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
  setGPIO(0, 0);
  sleep(1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
  setGPIO(0, 1);
  sleep(1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
  setGPIO(1, 0);
  sleep(1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
  setGPIO(1, 1);
  sleep(1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
}

void MainApplication::stateChanged(QProcess::ProcessState newstate)
{
    qDebug()<<__LINE__<<__FUNCTION__<<newstate;
}
void MainApplication::readGPIOerror()
{
    gpioErrorResponse = gpioProcess->readAllStandardError();
    qDebug()<<__LINE__<<__FUNCTION__<<gpioErrorResponse;
}
void MainApplication::readGPIO()
{
    gpioResponse = gpioProcess->readAllStandardOutput();
    qDebug()<<__LINE__<<__FUNCTION__<<gpioResponse;
}
void MainApplication::onFinish(int exitStatus)
{
    qDebug()<<__LINE__<<__FUNCTION__<<"GPIO finished:"<<exitStatus;
}

void MainApplication::startedGPIO()
{
    qDebug()<<__LINE__<<__FUNCTION__<<"Started GPIO";
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
  gpioProcess->setProgram(command);
  gpioProcess->setArguments(arguments);
  while (gpioProcess->state() == QProcess::NotRunning) {
    gpioProcess->start();
  }
  qDebug() << __LINE__ << __FUNCTION__ << "i2cset started " << pin3 << ","
           << pin4;
  gpioProcess->waitForReadyRead(-1);
  QString response = gpioProcess->readAllStandardOutput();
  qDebug() << __LINE__ << __FUNCTION__ << response;
  gpioProcess->waitForFinished(-1);
}
