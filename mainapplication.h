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
    void readGPIO();
    void readGPIOerror();
    void stateChanged(QProcess::ProcessState newstate);
    void onFinish(int exitCode, QProcess::ExitStatus exitStatus);
    void gpioErrorOccurred(QProcess::ProcessError error);
};

#endif // MAINAPPLICATION_H
