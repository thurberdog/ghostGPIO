#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QObject>
#include <QDebug>
#include <QProcess>
class MainApplication : public QObject
{
    Q_OBJECT
    QProcess i2cset;
    QProcess i2cget;
    void setGPIO(bool pin0, bool pin1);
    QString getGPIO();
public:
    explicit MainApplication(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MAINAPPLICATION_H
