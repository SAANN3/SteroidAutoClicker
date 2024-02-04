#ifndef SLEEPJOB_H
#define SLEEPJOB_H

#include <QObject>
#include <QQmlEngine>
#include "abstractjob.h"

class SleepJob : public AbstractJob
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SleepJob(QObject *parent = nullptr);
    void work();
};

#endif // SLEEPJOB_H
