
#include "gpio.hpp"

/**
 * @brief GPIO::GPIO
 * @param parent
 * @author Louis P. Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
GPIO::GPIO(QObject* parent) : QObject(parent)
{
        trace = true;
        //  i2cget, i2cset command syntax:
        //                  -y subpress interactive mode
        //                  -r Read back the value right after writing it, and compare
        //                  the result with the value written.
        //                   0: i2c-0
        //                   0x3E: slave address
        //                   0x__: command register bits
        //
        gpioProcess = new QProcess(this);
        gpioProcess->start(configure_gpio_command);
        timerLED = new QTimer(this); // create a timer
        connect(timerLED, SIGNAL(timeout()), this, SLOT(turnOffLED()));
        connect(gpioProcess, SIGNAL(started()), this, SLOT(startedGPIO()));
        connect(gpioProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                        SLOT(onFinish(int, QProcess::ExitStatus)));
        connect(gpioProcess, SIGNAL(readyReadStandardOutput()), this,
                        SLOT(readGPIO()));
        connect(gpioProcess, SIGNAL(readyReadStandardError()), this,
                        SLOT(readGPIOerror()));
        connect(gpioProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this,
                        SLOT(stateChanged(QProcess::ProcessState)));

}



/**
 * @brief GPIO::stateChanged
 * @param newstate
 * @author Louis P. Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::stateChanged(QProcess::ProcessState newstate)
{

        switch (newstate)
        {
        case QProcess::NotRunning:

                qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process is not running.";

                break;
        case QProcess::Starting:
                qDebug() << __LINE__ << __FUNCTION__
                                 << "The GPIO process is starting, but the program has not yet been invoked.";
                break;
        case QProcess::Running:
                qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process is running and is ready for reading and writing.";
                break;
        }
}

/**
 * @brief GPIO::readGPIOerror
 * @date November 6th 2019
 * @author Louis P. Meadows
 * @copyright Sciton
 */
void GPIO::readGPIOerror()
{
        gpioErrorResponse = gpioProcess->readAllStandardError();
        qDebug() << __LINE__ << __FUNCTION__ << "Error:" << gpioErrorResponse;

}

/**
 * @brief GPIO::readGPIO
 * @date November 6th 2019
 * @author Louis P. Meadows
 * @copyright Sciton
 */
void GPIO::readGPIO()
{
        gpioResponse = gpioProcess->readAllStandardOutput();
        qDebug() << __LINE__ << __FUNCTION__ << gpioResponse;

}

/**
 * @brief GPIO::onFinish
 * @param exitCode
 * @param exitStatus
 * @author Louis P. Meadows
 * @copyright Sciton
 */
void GPIO::onFinish(int exitCode, QProcess::ExitStatus exitStatus)
{

        switch (exitStatus)
        {
        case QProcess::NormalExit:
                qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process exited normally.";
                break;
        case QProcess::CrashExit:
                qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process crashed.";
                break;
        }
        qDebug() << __LINE__ << __FUNCTION__ << "Exit code:" << exitCode;
        qDebug() << __LINE__ << __FUNCTION__ << "Exit status:" << exitStatus;
}

/**
 * @brief GPIO::turnOffLED
 * @author Louis P Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::turnOffLED()
{
        gpioProcess->write("i2cset -y -r 0 0x3E 1 0");
        timerLED->stop(); // only run when LED is on
}
/**
 * @brief GPIO::startedGPIO
 * @author Louis P. Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::startedGPIO()
{
        qDebug() << __LINE__ << __FUNCTION__ << "Started GPIO process.";
}



/**
 * @brief GPIO::setGPIO
 * @param pin3
 * @param pin4
 * @author Louis P. Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::setGPIO(bool pin3, bool pin4)
{
        // pin 0 & 1 input from the controller
        // pin 2 & 3 output to the controller

        if ((pin3) && (pin4))
        {
                laserFireCount++;
                timerLED->start();
                gpioProcess->write("i2cset -y -r 0 0x3E 1 12");
                qDebug() << "=============================================="
                                 "==========================================";
                qDebug() << "====================================== Firing "
                                 "LASER ====================================";
                qDebug() << "=============================================="
                                 "==========================================";
                qDebug() << "LASER Has fired " << laserFireCount << " times";
        }
        if ((!pin3) && (pin4))
        {
                gpioProcess->write("i2cset -y -r 0 0x3E 1 8");
        }
        if ((pin3) && (!pin4))
        {
                gpioProcess->write("i2cset -y -r 0 0x3E 1 4");
        }
        if ((!pin3) && (!pin4))
        {
                gpioProcess->write("i2cset -y -r 0 0x3E 1 0");
                timerLED->stop();
        }

        qDebug() << __LINE__ << __FUNCTION__ << "i2cset started " << pin3 << "," << pin4;
}




