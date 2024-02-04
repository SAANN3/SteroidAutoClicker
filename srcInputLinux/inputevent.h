#ifndef INPUTEVENT_H
#define INPUTEVENT_H
#include <QObject>
#include <QTcpServer>
#include <unistd.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <fcntl.h>



class InputEvent :  public QObject
{
    Q_OBJECT
public:
    InputEvent(QObject *parent = nullptr);
    int main();
    void mouseInit(int width, int height);
    void absSetup(int width,int height);
    void closeInput();
    void event(int type, int code, int val);
private:
    bool setUp = false;
    QTcpServer *server;
    int fd = 0;
    void moveMouse(int x,int y,int type);
    void scrollMouse(int val);
private slots:
    void onReadyRead();
    void exitProgram();
    void newConnection();

};

#endif // INPUTEVENT_H
