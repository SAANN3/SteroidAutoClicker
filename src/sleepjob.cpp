#include "sleepjob.h"
SleepJob::SleepJob(QObject *parent)
    : AbstractJob{AbstractJob::TypeTimer,parent}
{}

void SleepJob::work()
{
    sleep();
}
