#include "mainapplication.h"

/**
 * @brief MainApplication::MainApplication
 * @author Louis P. Meadows
 * @date October 4th 2019
 * @version 1.0.0
 * @param parent
 */
MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  timer = new QTimer(this); // create a timer
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(milliseconds);
  gpio = new GPIO(this);
  gpio->configureGPIO();

}

void MainApplication::update()
{   switch (leds) {
    case 0:
        qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
        gpio->setGPIO(0, 0);
        qDebug() << __LINE__ << __FUNCTION__ << "GPIO:" << gpio->getGPIO();
        break;
    case 1:
        qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
        gpio->setGPIO(0, 1);
        qDebug() << __LINE__ << __FUNCTION__ << "GPIO:" << gpio->getGPIO();
        break;
    case 2:
        qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
        gpio->setGPIO(1, 0);
        qDebug() << __LINE__ << __FUNCTION__ <<  "GPIO:" << gpio->getGPIO();
        break;
    case 3:
        qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
        gpio->setGPIO(1, 1);
        qDebug() << __LINE__ << __FUNCTION__ <<  "GPIO:" << gpio->getGPIO();
        break;
    default:
        leds = -1;
    }
    leds++;


}




