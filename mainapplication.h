#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include <unistd.h>
#include <QObject>
#include <QDebug>
#include <QProcess>
class MainApplication : public QObject
{
    Q_OBJECT
    QProcess *gpioProcess;
    QProcess i2cget;
    void setGPIO(bool pin0, bool pin1);
    QString getGPIO();
    QString gpioResponse;
    QString gpioErrorResponse;
public:
    explicit MainApplication(QObject *parent = nullptr);

signals:

public slots:
    void startedGPIO();
    void onFinish(int exitStatus);
    void readGPIO();
    void readGPIOerror();
    void stateChanged(QProcess::ProcessState newstate);
};

#endif // MAINAPPLICATION_H
