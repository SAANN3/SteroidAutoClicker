#include "clickerjob.h"
#include <QJsonArray>
#include <QJsonDocument>
ClickerJob::ClickerJob(QObject *parent)
    : AbstractJob{AbstractJob::TypeClicker,parent}
{

}
void ClickerJob::setX(int x)
{
    this->x = x;
    emit xChanged();
}
void ClickerJob::setY(int y)
{
    this->y = y;
    emit yChanged();
}

void ClickerJob::setSide(int var)
{
    this->side = var;
    emit sideChanged();
}
int ClickerJob::getX()
{
    return x;
}
int ClickerJob::getY()
{
    return y;
}

int ClickerJob::getSide()
{
    return side;
}
void ClickerJob::work()
{
    #ifdef __linux__
    QString message = QString::number(getType()) + " " + QString::number(x) + " " + QString::number(y) + " " + QString::number(side);
    emit needToSend(message);
    sleep();
    #endif
    #ifdef _WIN32
    SetCursorPos(x,y);
    if(side==-1){
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
    else if(side == 1){
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }

    sleep();
    #endif
}
