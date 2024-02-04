#include "mousewheeljob.h"
MouseWheelJob::MouseWheelJob(QObject *parent)
    : AbstractJob{AbstractJob::TypeMouseScroll,parent}
{

}

bool MouseWheelJob::getDirection()
{
    return direction;
}

void MouseWheelJob::setDirection(bool val)
{
    direction = val;
}
void MouseWheelJob::work()
{
    #ifdef __linux__
    QString message = QString::number(getType()) + " " + QString::number(direction ? 1 : -1) ;
    emit needToSend(message);
    sleep();
    #endif
    #ifdef _WIN32
    mouse_event(MOUSEEVENTF_WHEEL,0,0,DWORD((direction?1:-1)*120),0);
    sleep();
    #endif
}
