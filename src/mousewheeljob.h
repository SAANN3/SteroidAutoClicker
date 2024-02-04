#ifndef MOUSEWHEELJOB_H
#define MOUSEWHEELJOB_H

#include <QObject>
#include <QQmlEngine>
#include "abstractjob.h"

class MouseWheelJob : public AbstractJob
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool direction  READ getDirection WRITE setDirection NOTIFY directionChanged)
public:
    explicit MouseWheelJob(QObject *parent = nullptr);
    bool getDirection();
    void setDirection(bool val);
    void work();

private:
    bool direction = true;
signals:
    void directionChanged();
};

#endif // MOUSEWHEELJOB_H
