
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
        gpioProcess = new QProcess(this);
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

void GPIO::unexport()
{
        // Unexport the pin by writing to /sys/class/gpio/unexport

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd == -1)
        {
                perror("Unable to open /sys/class/gpio/unexport");
                exit(1);
        }

        if (write(fd, "3e", 2) != 2)
        {
                perror("Error writing to /sys/class/gpio/unexport");
                exit(1);
        }

        close(fd);
}
void GPIO::togglePin()
{
        fd = open("/sys/class/gpio/gpio62/value", O_WRONLY);
        if (fd == -1)
        {
                perror("Unable to open /sys/class/gpio/gpio62/value");
                exit(1);
        }

        // Toggle LED 50 ms on, 50ms off, 100 times (10 seconds)
        for (int i = 0; i < 100; i++)
        {
                if (write(fd, "1", 1) != 1)
                {
                        perror("Error writing to /sys/class/gpio/gpio62/value");
                        exit(1);
                }
                usleep(50000);

                if (write(fd, "0", 1) != 1)
                {
                        perror("Error writing to /sys/class/gpio/gpio62/value");
                        exit(1);
                }
                usleep(50000);
        }

        close(fd);
}
void GPIO::setDirection()
{
        // Set the pin to be an output by writing "out" to
        // /sys/class/gpio/gpio24/direction
        fd = open("/sys/class/gpio/gpio3e/direction", O_WRONLY);
        if (fd == -1)
        {
                perror("Unable to open /sys/class/gpio/gpio62/direction");
                exit(1);
        }

        if (write(fd, "out", 3) != 3)
        {
                perror("Error writing to /sys/class/gpio/gpio62/direction");
                exit(1);
        }

        close(fd);
}
void GPIO::exportPins()
{
        // Export the desired pin by writing to /sys/class/gpio/export

        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd == -1)
        {
                perror("Unable to open /sys/class/gpio/export");
                exit(1);
        }

        if (write(fd, "62", 2) != 2)
        {
                perror("Error writing to /sys/class/gpio/export");
                exit(1);
        }
        close(fd);
}

void GPIO::print_usage(void)
{
        fprintf(stderr, "Usage: lsgpio [options]...\n"
                        "List GPIO chips, lines and states\n"
                        "  -n <name>  List GPIOs on a named device\n"
                        "  -?         This helptext\n");
}

/**
 * @brief GPIO::configureGPIO
 * @author Louis P. Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::configureGPIO()
{

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
        gpioProcess->waitForReadyRead(milliseconds);
        QString cmdOutput =
                gpioProcess->readAllStandardOutput(); // Reads standard output
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << cmdOutput;
        }
//        if (gpioProcess->state() != QProcess::NotRunning)
//        {
//                // Returns true if the process finished; otherwise returns false (if the
//                // operation timed out, if an error occurred, or if this QProcess is already
//                // finished).
//                if (gpioProcess->waitForFinished(milliseconds) == false)
//                {
//                        gpioProcess->terminate();
//                        gpioProcess->kill();
//                }
//        }
        gpioProcess->start(clear_gpio);
        gpioProcess->waitForReadyRead(milliseconds);
        cmdOutput = gpioProcess->readAllStandardOutput(); // Reads standard output
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << cmdOutput;
        }
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

                //    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process is not
                //    running.";

                break;
        case QProcess::Starting:
                //    qDebug() << __LINE__ << __FUNCTION__
                //             << "The GPIO process is starting, but the program has not yet
                //             "
                //                "been invoked.";
                break;
        case QProcess::Running:
                //    qDebug()
                //        << __LINE__ << __FUNCTION__
                //        << "The GPIO process is running and is ready for reading and
                //        writing.";
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
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << "Error:" << gpioErrorResponse;
        }
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
        if (gpioResponse.contains("0x"))
        {
                fireSwitch = gpioResponse;
        }
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << gpioResponse;
        }
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
                //    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process exited
                //    normally.";
                break;
        case QProcess::CrashExit:
                //    qDebug() << __LINE__ << __FUNCTION__ << "The GPIO process crashed.";
                break;
        }
        qDebug() << __LINE__ << __FUNCTION__ << "Exit code:" << exitCode;
        //  qDebug() << __LINE__ << __FUNCTION__ << "Exit status:" << exitStatus;
}

/**
 * @brief GPIO::turnOffLED
 * @author Louis P Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
void GPIO::turnOffLED()
{
        gpioProcess->start("i2cset -y -r 0 0x3E 1 0");
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
        //  qDebug() << __LINE__ << __FUNCTION__ << "Started GPIO process.";
}

/**
 * @brief GPIO::readInput
 * @return
 * @author Louis P Meadows
 * @date November 13th 2019
 * @copyright Sciton
 *
 */
QString GPIO::readInput()
{
        gpioProcess->start("i2cget -y 0 0x3E 0");
        gpioProcess->waitForReadyRead(-1);
        gpioInput = gpioProcess->readAllStandardOutput();
        gpioProcess->waitForFinished(milliseconds);
        return gpioInput;
}
/**
 * @brief GPIO::getGPIO
 * @return
 * @author Louis P. Meadows
 * @date November 6th 2019
 * @copyright Sciton
 */
QString GPIO::getGPIO()
{
        QString registerGPIO;
        gpioProcess->start(read_gpio_input);
        gpioProcess->waitForReadyRead(-1);
        gpioInput = gpioProcess->readAllStandardOutput();
        gpioProcess->waitForFinished(milliseconds);
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << read_gpio_input;
                qDebug() << __LINE__ << __FUNCTION__ << gpioInput;
        }
        gpioProcess->start(read_gpio_output);
        gpioProcess->waitForReadyRead(-1);
        gpioOutput = gpioProcess->readAllStandardOutput();
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << read_gpio_output;
                qDebug() << __LINE__ << __FUNCTION__ << gpioOutput;
        }
        gpioProcess->waitForFinished(milliseconds);
        if ((gpioInput.contains("0x02")) || (gpioInput.contains("0x0f")) ||
                        (gpioInput.contains("0x07")) || (gpioInput.contains("0x0a")))
        {
                fireSwitch = gpioInput;
                if (trace)
                {
                        qDebug() << __LINE__ << __FUNCTION__ << "FIRE SWITCH PRESSED";
                }
        }
        else
        {
                fireSwitch = "0x00";
        }
        return gpioInput;
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

                gpioProcess->start("i2cset -y -r 0 0x3E 1 12");
        }
        if ((!pin3) && (pin4))
        {
                gpioProcess->start("i2cset -y -r 0 0x3E 1 8");
        }
        if ((pin3) && (!pin4))
        {
                gpioProcess->start("i2cset -y -r 0 0x3E 1 4");
        }
        if ((!pin3) && (!pin4))
        {
                gpioProcess->start("i2cset -y -r 0 0x3E 1 0");
        }

        qDebug() << __LINE__ << __FUNCTION__ << "i2cset started " << pin3 << ","
                         << pin4 << gpioProcess->readAllStandardOutput();
        gpioProcess->waitForFinished(milliseconds);
}

/**
 * @brief GPIO::fireLaser
 * @author Louis P Meadows
 * @date November 13th 2019
 * @copyright Sciton
 */
bool GPIO::fireLaser()
{
        bool fire = false;
        if (trace)
        {
                qDebug() << __LINE__ << __FUNCTION__ << fireSwitch;
        }
        if (fireSwitch.contains("0x01") || (fireSwitch.contains("0x02")) ||
                        (fireSwitch.contains("0x03")))
        {
                fire = true;
                gpioProcess->start("i2cset -y -r 0 0x3E 1 4");
                timerLED->start(50);
                if (trace)
                {
                        qDebug() << "=============================================="
                                         "==========================================";
                        qDebug() << "====================================== Firing "
                                         "LASER ====================================";
                        qDebug() << "=============================================="
                                         "==========================================";
                        qDebug() << "LASER Has fired " << laserFireCount << " times";
                }
                gpioProcess->waitForFinished(-1);
                laserFireCount++;
        }
        else
        {
                if (trace)
                {
                        qDebug() << __LINE__ << __FUNCTION__ << "Fire switch not pressed";
                }
        }
        return fire;
}

/**
 * @brief GPIO::clear
 * @author Louis P Meadows
 * @date November 6th 2019
 * @copyright Sciton
 */
void GPIO::clear()
{
        gpioProcess->start("i2cset -y -r 0 0x3E 1 0");
        gpioProcess->waitForFinished(-1);
}
