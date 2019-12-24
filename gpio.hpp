#ifndef GPIO_HPP
#define GPIO_HPP
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <getopt.h>

// #include <linux/gpio/driver.h>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <sys/ioctl.h>
class GPIO : public QObject
{
        Q_OBJECT
public:
        explicit GPIO(QObject* parent = nullptr);
        static inline int check_prefix(const char* str, const char* prefix)
        {
                return strlen(str) > strlen(prefix) &&
                           strncmp(str, prefix, strlen(prefix)) == 0;
        }
        struct gpio_flag
        {

                char const* name;

                unsigned long mask;
        };
        const char* device_name = NULL;
        int ret;
        int c;
        bool trace = false;
        QProcess* gpioProcess;
        void setGPIO(bool pin0, bool pin1);
        // There are three required options to i2cset. i2cbus indicates the number or
        // name of the I2C bus to be scanned. This number should correspond to one of
        // the busses listed by i2cdetect -l. chip-address specifies the address of the
        // chip on that bus, and is an integer between 0x03 and 0x77. data-address
        // specifies the address on that chip to write to, and is an integer between
        // 0x00 and 0xFF.
        uint gpio = 0x3e;
        int fd;
        QString getGPIO();
        QString gpioResponse;
        QString gpioInput;
        QString gpioOutput;
        QString gpioErrorResponse;
        QString clear_gpio = "i2cset -y -r 0 0x3E 1 0";
        QString configure_gpio_command = "i2cset -y -r 0 0x3e 3 0x03";
        QString read_gpio_output = "i2cget -y 0 0x3E 1";
        QString read_gpio_input = "i2cget -y 0 0x3E 0";
        QString fireSwitch;
        QTimer* timerLED;
        int milliseconds = 1000; // 1 second
        int laserFireCount = 0;
        void configureGPIO();
        void clear();
        QString readInput();
        bool fireLaser();
        void print_flags(unsigned long flags);
        int list_device(const char* device_name);
        void print_usage();
        void exportPins();
        void setDirection();
        void togglePin();
        void unexport();
        void listDevices();
signals:

public slots:
        void startedGPIO();
        void readGPIO();
        void readGPIOerror();
        void stateChanged(QProcess::ProcessState newstate);
        void onFinish(int exitCode, QProcess::ExitStatus exitStatus);
        void turnOffLED();

private:
};

#endif // GPIO_HPP
