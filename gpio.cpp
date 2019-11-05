#include "gpio.hpp"

GPIO::GPIO(QObject *parent) : QObject(parent) {
  gpioProcess = new QProcess(this);
  connect(gpioProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this,
          SLOT(stateChanged(QProcess::ProcessState)));
  connect(gpioProcess, SIGNAL(readyReadStandardError()), this,
          SLOT(readGPIOerror()));
  connect(gpioProcess, SIGNAL(readyReadStandardOutput()), this,
          SLOT(readGPIO()));
  connect(gpioProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
          SLOT(onFinish(int, QProcess::ExitStatus)));
  connect(gpioProcess, SIGNAL(started()), this, SLOT(startedGPIO()));
}

/**
 * @brief GPIO::configureGPIO
 * @date November 4th 2019
 * @author Louis P. Meadows
 */
void GPIO::configureGPIO() {

  //  i2cget, i2cset command syntax:
  //                  -y subpress interactive mode
  //                  -r Read back the value right after writing it, and compare
  //                  the result with the value written.
  //                   0: i2c-0
  //                   0x3E: slave address
  //                   0x__: command register bits
  //

  gpioProcess->start(configure_gpio_command);
  gpioProcess->waitForBytesWritten(milliseconds);
  gpioProcess->waitForFinished(milliseconds);
  gpioProcess->waitForReadyRead(milliseconds);
  QString cmdOutput =
      gpioProcess->readAllStandardOutput(); // Reads standard output
  qDebug() << __LINE__ << __FUNCTION__ << cmdOutput;
  if (gpioProcess->state() != QProcess::NotRunning) {
    // Returns true if the process finished; otherwise returns false (if the
    // operation timed out, if an error occurred, or if this QProcess is already
    // finished).
    if (gpioProcess->waitForFinished(milliseconds) == false) {
      gpioProcess->terminate();
      gpioProcess->kill();
    }
  }
  gpioProcess->start(clear_gpio);
  gpioProcess->waitForReadyRead(milliseconds);
  cmdOutput = gpioProcess->readAllStandardOutput(); // Reads standard output
  qDebug() << __LINE__ << __FUNCTION__ << cmdOutput;
}

void GPIO::setGPIO(bool pin3, bool pin4) {
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

QString GPIO::getGPIO() {
  QString pins;
  gpioProcess->start("i2cget -y 0 0x3E 1");
  pins = gpioProcess->readAllStandardOutput();
  qDebug() << __LINE__ << __FUNCTION__ << "GPIO:" << pins;
  return pins;
}

void GPIO::stateChanged(QProcess::ProcessState newstate) {

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

void GPIO::readGPIOerror() {
  QString gpioErrorResponse = gpioProcess->readAllStandardError();
  qDebug() << __LINE__ << __FUNCTION__ << gpioErrorResponse;
}
void GPIO::readGPIO() {
  QString gpioResponse = gpioProcess->readAllStandardOutput();
  qDebug() << __LINE__ << __FUNCTION__ << gpioResponse;
}

void GPIO::onFinish(int exitCode, QProcess::ExitStatus exitStatus) {
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

void GPIO::startedGPIO() {
  qDebug() << __LINE__ << __FUNCTION__ << "Started GPIO process.";
}
