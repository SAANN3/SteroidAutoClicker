#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include <QObject>
#include <QQmlEngine>
#include "abstractjob.h"
#include "QTcpSocket"
class AutoClicker : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QQmlListProperty<AbstractJob> jobs READ getJobs NOTIFY jobsChanged)
    Q_PROPERTY(bool working  READ getWorking NOTIFY workingChanged)
    Q_PROPERTY(QString globalPos READ getGlobalPos NOTIFY globalPosChanged)
    Q_PROPERTY(int repeatAmount READ getRepeatAmount WRITE setRepeatAmount NOTIFY repeatAmountChanged)
public:
    explicit AutoClicker(QObject *parent = nullptr);
    Q_INVOKABLE void startWork();
    Q_INVOKABLE void stopWork();
    Q_INVOKABLE void guiExited();
    Q_INVOKABLE void addItem(int type);
    Q_INVOKABLE void deleteItem(int index);
    Q_INVOKABLE void workWithFile(QString fileName,int typeOfWork);

    QQmlListProperty<AbstractJob> getJobs();
    QString getGlobalPos();
    void setRepeatAmount(int val);
    int getRepeatAmount();
    static QTcpSocket *getSocket();
    static bool getWorking();
    void jobsWork();

private:
    void exportFile(QString fileName);
    void importFile(QString fileName);
    QList<AbstractJob*> jobs;
    int repeatAmount = 1;
    static inline bool working = false;
    static inline QTcpSocket *socket = nullptr;
    bool guiAlive = true;
    QString globalPos;
    void setCursorPositionWay();
    void getDisplayRes();
private slots:
    void connectionAborted();
signals:
    void repeatAmountChanged();
    void globalPosChanged();
    void workingChanged();
    void jobsChanged();
};
#endif // AUTOCLICKER_H
