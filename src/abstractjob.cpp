#include "abstractjob.h"
#include "autoclicker.h"
#include <QTcpSocket>
AbstractJob::AbstractJob(int type,QObject *parent)
    : QObject{parent}
{
    this->type = type;
    connect(this,SIGNAL(needToSend(QString)),this,SLOT(sendData(QString)));
}

void AbstractJob::workWrapper()
{
    setIsWorking(true);
    currentAmount = 0;
    for(currentAmount = 0;currentAmount<amount;currentAmount++)
    {
        if(!AutoClicker::getWorking()){
            break;
        }
        work();
    }
    setIsWorking(false);
}

void AbstractJob::setAmount(int amount)
{
    this->amount = amount;
    emit amountChanged();
}
void AbstractJob::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged();
}
void AbstractJob::setIsWorking(bool state)
{
    this->isWorking = state;
    emit isWorkingChanged();
}

void AbstractJob::sleep()
{
    QThread::msleep(delay);
}
int AbstractJob::getAmount()
{
    return amount;
}
int AbstractJob::getDelay()
{
    return delay;
}
int AbstractJob::getType()
{
    return type;
}

bool AbstractJob::getIsWorking()
{
    return isWorking;
}

void AbstractJob::sendData(QString data)
{
    AutoClicker::getSocket()->write(data.toUtf8());
    AutoClicker::getSocket()->waitForBytesWritten();
}
