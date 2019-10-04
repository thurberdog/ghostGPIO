#include "mainapplication.h"

/**
 * @brief MainApplication::MainApplication
 * @author Louis P. Meadows
 * @date October 4th 2019
 * @version 1.0.0
 * @param parent
 */
MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  gpioProcess = new QProcess();
  connect(gpioProcess, SIGNAL(started()), this, SLOT(startedGPIO()));
  connect(gpioProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
          SLOT(onFinish(int, QProcess::ExitStatus)));
  connect(gpioProcess, SIGNAL(readyReadStandardOutput()), this,
          SLOT(readGPIO()));
  connect(gpioProcess, SIGNAL(readyReadStandardError()), this,
          SLOT(readGPIOerror()));
  connect(gpioProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this,
          SLOT(stateChanged(QProcess::ProcessState)));

  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
  setGPIO(0, 0);
  gpioProcess->waitForFinished(-1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
  setGPIO(0, 1);
  gpioProcess->waitForFinished(-1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
  setGPIO(1, 0);
  gpioProcess->waitForFinished(-1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
  setGPIO(1, 1);
  gpioProcess->waitForFinished(-1);
  qDebug() << __LINE__ << __FUNCTION__ << getGPIO();
}

/**
 * @brief MainApplication::stateChanged
 * @author Louis P. Meadows
 * @param newstate
 */
void MainApplication::stateChanged(QProcess::ProcessState newstate) {

  switch (newstate) {
  case QProcess::NotRunning:
    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process is not running.";
    break;
  case QProcess::Starting:
    qDebug() << __LINE__ << __FUNCTION__
             << "The GPIO process is starting, but the program has not yet "
                "been invoked.";
    break;
  case QProcess::Running:
    qDebug()
        << __LINE__ << __FUNCTION__
        << "The GPIO process is running and is ready for reading and writing.";
    break;
  }
}

/**
 * @brief MainApplication::readGPIOerror
 * @author Louis P. Meadows
 */
void MainApplication::readGPIOerror() {
  gpioErrorResponse = gpioProcess->readAllStandardError();
  qDebug() << __LINE__ << __FUNCTION__ << gpioErrorResponse;
}

/**
 * @brief MainApplication::readGPIO
 * @author Louis P. Meadows
 */
void MainApplication::readGPIO() {
  gpioResponse = gpioProcess->readAllStandardOutput();
  qDebug() << __LINE__ << __FUNCTION__ << gpioResponse;
}

/**
 * @brief MainApplication::onFinish
 * @param exitCode
 * @param exitStatus
 * @author Louis P. Meadows
 */
void MainApplication::onFinish(int exitCode, QProcess::ExitStatus exitStatus) {
  qDebug() << __LINE__ << __FUNCTION__ << "GPIO finished:";
  qDebug("Exit code: %i", exitCode);
  qDebug("Exit status: %i", exitStatus);

  switch (exitStatus) {
  case QProcess::NormalExit:
    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process exited normally.";
    break;
  case QProcess::CrashExit:
    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process crashed.";
    break;
  }
}

/**
 * @brief MainApplication::startedGPIO
 * @author Louis P. Meadows
 */
void MainApplication::startedGPIO() {
  qDebug() << __LINE__ << __FUNCTION__ << "Started GPIO process.";
}

/**
 * @brief MainApplication::getGPIO
 * @author Louis P. Meadows
 * @return
 */
QString MainApplication::getGPIO() {
  QString pins;
  QString command = "i2cget";
  QStringList arguments;
  arguments << "-y";
  arguments << "0";
  arguments << "0x3E";
  arguments << "0";
  gpioProcess->setProgram(command);
  gpioProcess->setArguments(arguments);
  gpioProcess->start();
  gpioProcess->waitForReadyRead();
  pins = gpioProcess->readAllStandardOutput();
  gpioProcess->waitForFinished(-1);
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
    qDebug() << __LINE__ << __FUNCTION__ << "i2cset started " << pin3 << ","
             << pin4;
  }
  gpioProcess->waitForFinished(-1);
}
