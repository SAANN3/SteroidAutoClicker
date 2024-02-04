#ifndef CLICKERJOB_H
#define CLICKERJOB_H

#include <QObject>
#include <QQmlEngine>
#include "abstractjob.h"

class ClickerJob : public AbstractJob
{

    Q_OBJECT
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY xChanged);
    Q_PROPERTY(int y READ getY WRITE setY NOTIFY yChanged);
    Q_PROPERTY(int side READ getSide WRITE setSide NOTIFY sideChanged);
    QML_ELEMENT
public:
    explicit ClickerJob(QObject *parent = nullptr);
    void work();
    void setX(int x);
    void setY(int x);
    void setSide(int var);
    int getX();
    int getY();
    int getSide();


private:
    int side = -1;
    int x = 0;
    int y = 0;
signals:
    void sideChanged();
    void xChanged();
    void yChanged();
};
#endif // CLICKERJOB_H
