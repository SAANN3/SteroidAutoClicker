#ifndef ABSTRACTJOB_H
#define ABSTRACTJOB_H

#include <QObject>
#include <QQmlEngine>
#include <QThread>
#include <QByteArray>
#ifdef _WIN32
#include <windows.h>
#include <winable.h>
#include <winuser.h>
#endif
class AbstractJob : public QObject
{

    Q_OBJECT
    Q_PROPERTY(int amount READ getAmount WRITE setAmount NOTIFY amountChanged);
    Q_PROPERTY(int delay READ  getDelay WRITE setDelay NOTIFY delayChanged);
    Q_PROPERTY(bool isWorking READ getIsWorking  NOTIFY isWorkingChanged)
    Q_PROPERTY(int type READ  getType CONSTANT);
    QML_ELEMENT
    QML_UNCREATABLE("Abstract class");
public:
    explicit AbstractJob(int type,QObject *parent = nullptr);
    void workWrapper();
    virtual void work() = 0;
    void setAmount(int amount);
    void setDelay(int amount);
    void setIsWorking(bool state);
    void sleep();
    int getAmount();
    int getDelay();
    int getType();
    bool getIsWorking();
    static const int TypeTimer = 0;
    static const int TypeClicker = 1;
    static const int TypeMouseScroll = 2;

private:
    int type;
    int amount = 1;
    int delay = 1;
    int currentAmount = 0;
    bool isWorking = false;

signals:
    void isWorkingChanged();
    void amountChanged();
    void delayChanged();
    void needToSend(QString data);
private slots:
    void sendData(QString data);
};

#endif // ABSTRACTJOB_H
