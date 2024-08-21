#include "inputevent.h"
#include <cstring>
#include <QTcpSocket>
#include <unistd.h>
#include <QThread>
#include <QStringList>
#include <unistd.h>

InputEvent::InputEvent(QObject *parent)
    : QObject{parent}
{
    if(getuid()){
        qDebug() << "You don't have root rights";
        qDebug() << "This program runs only with root";
        exit(1);
    }
    if(!getuid()){
        qDebug() << "Running as root";

    }
    QString a;
    a.unicode();
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::LocalHost,30123)){
        qDebug() << server->errorString();
        closeInput();
        exit(1);
    }
    qDebug() << "Waiting for response from app";
}


void InputEvent::mouseInit(int width,int height)
{

    struct uinput_setup usetup;
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd == -1){
        qDebug() << "Couldn't open /dev/uniput file";
        exit(1);

    }
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);
    ioctl(fd, UI_SET_EVBIT, EV_ABS);
    absSetup(width,height);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234; /* sample vendor */
    usetup.id.product = 0x5678; /* sample product */
    strcpy(usetup.name, "InputEvent device");
    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
    sleep(1);
    setUp = true;

}

void InputEvent::absSetup(int width,int height)
{
    struct uinput_abs_setup abs = {
        .code =  ABS_X,
        .absinfo = {
            .minimum = 0,
            .maximum = width,
            .resolution = 1
        }
    };
    ioctl(fd, UI_ABS_SETUP, &abs);
    abs = {
        .code =  ABS_Y,
        .absinfo = {
            .minimum = 0,
            .maximum = height,
            .resolution = 1
        }
    };
    ioctl(fd, UI_ABS_SETUP, &abs);
}

void InputEvent::closeInput()
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void InputEvent::event(int type, int code, int val)
{
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
    if(write(fd, &ie, sizeof(ie)) < 0){
        qDebug() << type << code << val;
    }
}

void InputEvent::moveMouse(int x, int y,int type)
{
    /// reset mouse pos
    event(EV_ABS, ABS_X,-1);
    event(EV_ABS, ABS_Y,-1);
    ///
    event(EV_ABS, ABS_X,x);
    event(EV_ABS, ABS_Y,y);
    event(EV_SYN, SYN_REPORT, 0);
    if(type==-1){
        event(EV_KEY, BTN_LEFT,1);
        event(EV_SYN, SYN_REPORT, 0);
        event(EV_KEY, BTN_LEFT,0);
        event(EV_SYN, SYN_REPORT, 0);
    }
    else if(type == 1){
        event(EV_KEY, BTN_RIGHT,1);
        event(EV_SYN, SYN_REPORT, 0);
        event(EV_KEY, BTN_RIGHT,0);
        event(EV_SYN, SYN_REPORT, 0);
    }


}

void InputEvent::scrollMouse(int val)
{
    event(EV_REL,REL_WHEEL,val);
    event(EV_SYN, SYN_REPORT, 0);
}



void InputEvent::onReadyRead()
{
    QTcpSocket *senderSocket = (QTcpSocket*)(sender());
    if(senderSocket)
    {

        QByteArray rawData = senderSocket->readAll();
        QString data = QString(rawData);
        QStringList list = data.split(" ",Qt::SkipEmptyParts);
        QVector<int> vector;

        foreach(QString num,list){
            vector.push_back(num.toInt());
        }

        if(vector[0]==-1){

            mouseInit(vector[1],vector[2]);
            qDebug() << "Virtual device set up : " << vector[1] << vector[2];
            qDebug() << "Ready";
        }
        if(!setUp){
            qDebug() << "Device not set up, aborting";
            exit(1);
        }
        if(vector[0]==1){
            moveMouse(vector[1]+1,vector[2]+1,vector[3]);
        }
        if(vector[0]==2){
            scrollMouse(vector[1]);
        }
    }
}

void InputEvent::exitProgram()
{
    closeInput();
    qDebug() << "Connection from app was closed";
    exit(0);

}

void InputEvent::newConnection()
{
    QTcpSocket * newSocket = server->nextPendingConnection();

    if(newSocket)
    {
        connect(newSocket ,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(newSocket ,SIGNAL(disconnected()),newSocket ,SLOT(deleteLater()));
        connect(newSocket ,SIGNAL(disconnected()),this ,SLOT(exitProgram()));
    }
}

