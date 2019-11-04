#ifndef GPIO_HPP
#define GPIO_HPP

#include <QObject>
#include <QDebug>
#include <QProcess>

class GPIO : public QObject {
  Q_OBJECT
public:
  explicit GPIO(QObject *parent = nullptr);

  void setGPIO(bool pin3, bool pin4);
  QProcess *gpioProcess;
  QString clear_gpio = "i2cset -y -r 0 0x3E 1 0";
  QString configure_gpio_command = "i2cset -y -r 0 0x3e 0x03";
  QString read_gpio_pins =
      "i2cget -y 0 0x3E 0"; // read J22 pin 3 -- GP1 for FS line
  int milliseconds = 1000;  // 1 second
  void configureGPIO();
  QString getGPIO();
signals:

public slots:
  void stateChanged(QProcess::ProcessState newstate);
  void readGPIOerror();
  void readGPIO();
  void onFinish(int exitCode, QProcess::ExitStatus exitStatus);
  void startedGPIO();
};

#endif // GPIO_HPP
