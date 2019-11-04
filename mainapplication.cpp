﻿#include "mainapplication.h"

/**
 * @brief MainApplication::MainApplication
 * @author Louis P. Meadows
 * @date October 4th 2019
 * @version 1.0.0
 * @param parent
 */
MainApplication::MainApplication(QObject *parent) : QObject(parent) {
  gpio = new GPIO(this);
  gpio->configureGPIO();
  sleep(100);
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,0";
  gpio->setGPIO(0, 0);
  sleep(100);
  qDebug() << __LINE__ << __FUNCTION__ << "GPIO:" << gpio->getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 0,1";
  gpio->setGPIO(0, 1);
  sleep(100);
  qDebug() << __LINE__ << __FUNCTION__ <<  "GPIO:" << gpio->getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,0";
  gpio->setGPIO(1, 0);
  sleep(100);
  qDebug() << __LINE__ << __FUNCTION__ <<  "GPIO:" << gpio->getGPIO();
  qDebug() << __LINE__ << __FUNCTION__ << "Setting 1,1";
  gpio->setGPIO(1, 1);
  sleep(100);
  qDebug() << __LINE__ << __FUNCTION__ <<  "GPIO:" << gpio->getGPIO();
}

