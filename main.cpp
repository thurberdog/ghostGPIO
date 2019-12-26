
#include <QCoreApplication>
#include "gpio.hpp"

int main(int argc, char* argv[])
{
        QCoreApplication a(argc, argv);
        GPIO* gpio = new GPIO();
        for (int i = 0; i < 100; i++)
        {
                gpio->setGPIO(1, 1);
                gpio->setGPIO(1, 0);
                gpio->setGPIO(0, 1);
                gpio->setGPIO(0, 0);
        }
        return a.exec();
}
