#include "autoclicker.h"
#include "clickerjob.h"
#include "mousewheeljob.h"
#include "qcursor.h"
#include "sleepjob.h"
#include <QThreadPool>
#include <QProcessEnvironment>
#include <QProcess>
#include <QStringList>
#include <QUrl>
#include <QCursor>
#include <QFile>
#ifdef _WIN32
#include <windows.h>
#include <winable.h>
#endif

AutoClicker::AutoClicker(QObject *parent)
    : QObject{parent}
{

    setCursorPositionWay();
    #ifdef __linux__
    QThread::sleep(1);
    socket = new QTcpSocket();
    connect(socket,SIGNAL(disconnected()),this,SLOT(connectionAborted()));
    socket->connectToHost(QHostAddress::LocalHost,30123);
    if(!socket->waitForConnected(30000)){
        qDebug() << "couldn't connect to mover";
        qDebug() << socket->errorString();
        exit(1);
    }
    getDisplayRes();
    #endif
}

void AutoClicker::stopWork()
{
    working = false;
}
void AutoClicker::startWork()
{
    if(!working){
        working = true;
        emit workingChanged();
        jobsWork();
    }
}
void AutoClicker::jobsWork()
{
    QThreadPool::globalInstance()->start([this](){
        int currentAmount = 0;
        while((currentAmount<repeatAmount) && guiAlive){
            currentAmount++;
            for(int i =0;i<jobs.size();i++){
                if(working && guiAlive){
                    jobs[i]->workWrapper();
                }

                else{
                    break;
                }
            }
        }

        working = false;
        emit workingChanged();
    });
}


void AutoClicker::setCursorPositionWay()
{
    //AFAIK wayland has many components so there is different aproach to get pos
    //btw xorg also should be supported but probably need implementation for gtk
    //i think the easiest way gonna be launching system command and get output;
    //for adding your WM or DE feel free to create an issue or pull request with needed commands
    std::function<QString()>lamda; // empty lamda declaration
    bool defined = false; //check if implementantion was found
    if(QProcessEnvironment::systemEnvironment().contains("HYPRLAND_CMD")){
        defined = true;
        lamda = [](){
            QString command[2] = {"hyprctl","cursorpos"};
            QStringList params;
            params << command[1];
            QProcess process;
            process.start(command[0],params);
            process.waitForFinished();
            QString output(process.readAllStandardOutput());
            output.remove('\n'); // process adds new line on end
            QStringList list = output.split(", ",Qt::SkipEmptyParts);
            QVector<int> vector;
            foreach(QString num,list){
                vector.push_back(num.toInt());
            }
            QString returnVal ="Current cursor position is x:" + QString::number(vector[0]) + "y:" +QString::number(vector[1]);
            return returnVal;
        };
    }
    if(QProcessEnvironment::systemEnvironment().contains("DESKTOP_SESSION")){
        if(QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION")=="plasma"){
            defined = true;
            lamda = [](){
                QString returnVal ="Current cursor position is x:" + QString::number(QCursor::pos().rx()) + "y:" +QString::number(QCursor::pos().ry());
                return returnVal;
            };


        }
    }
    #ifdef _WIN32
    defined = true;
    lamda = [](){
        POINT cursor;
        GetCursorPos(&cursor);
        QString returnVal ="Current cursor position is x:" + QString::number(cursor.x) + "y:" +QString::number(cursor.y);
        return returnVal;
    };
    #endif


    //repeatedly update value;
    if(!defined){
        //means there is no implementation
        globalPos = "globalCursorPosition for your WM|DE isn't implemented yet . \nFeel free to contribute to this project, i'd appreciate it.";
        emit globalPosChanged();
        return;
    }
    // process which can constantly update our value;
    QThreadPool::globalInstance()->start([this, lamda](){
        // if there is need to call
        while(guiAlive){
            globalPos = lamda();
            emit globalPosChanged();
            QThread::msleep(250);
        }
        emit workingChanged();
    });
}

void AutoClicker::getDisplayRes()
{
    QString command[3] = {"/bin/bash","-c","xdpyinfo | awk '/dimensions:/ { print $2 }'"};//Credit: https://unix.stackexchange.com/a/2682/126755
    QStringList params;
    params << command[1];
    params << command[2];
    QProcess process;
    process.start(command[0],params);
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    output.remove('\n'); // process adds new line on end
    QStringList list = output.split("x",Qt::SkipEmptyParts);
    QVector<int> vector;
    foreach(QString num,list){
        vector.push_back(num.toInt());
    }
    QString writeData = "-1 " + QString::number(vector[0]) + " " + QString::number(vector[1]);
    socket->write(writeData.toUtf8());
    socket->waitForBytesWritten();
}

void AutoClicker::connectionAborted()
{
    qDebug() << "Mover closed connection";
    exit(1);
}


void AutoClicker::guiExited()
{
    guiAlive = false;
}

void AutoClicker::addItem(int type)
{
    AbstractJob *newJob;
    switch(type){
    case AbstractJob::TypeTimer:
        newJob = new SleepJob();
        break;
    case AbstractJob::TypeClicker:
        newJob = new ClickerJob();
        break;
    case AbstractJob::TypeMouseScroll:
        newJob = new MouseWheelJob();
        break;
    default:
        qDebug() << "This isn't possible";
        exit(1);
    }
    jobs.push_back(newJob);
    emit jobsChanged();

}

void AutoClicker::deleteItem(int index)
{
    AbstractJob *ptr = jobs[index];
    jobs.removeAt(index);
    emit jobsChanged();
    delete ptr;
}

void AutoClicker::workWithFile(QString fileName, int typeOfWork)
{
    QUrl url(fileName);
    fileName = url.toLocalFile();
    if(typeOfWork == 1){
        importFile(fileName);

    }
    if(typeOfWork == 2){
        exportFile(fileName);
    }
}

void AutoClicker::exportFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    QTextStream out(&file);
    foreach (AbstractJob *abstractJob, jobs) {
        out << (QString::number(abstractJob->getType()) + " " + QString::number(abstractJob->getAmount()) + " " +  QString::number(abstractJob->getDelay())) + " ";
        switch(abstractJob->getType()){
        case AbstractJob::TypeTimer:
            break;
        case AbstractJob::TypeClicker:
            out << QString::number(((ClickerJob*)abstractJob)->getX()) << " ";
            out << QString::number(((ClickerJob*)abstractJob)->getY()) << " ";
            out << QString::number(((ClickerJob*)abstractJob)->getSide());
            break;
        case AbstractJob::TypeMouseScroll:
            out <<QString::number(((MouseWheelJob*)abstractJob)->getDirection());
            break;
        default:
            qDebug() << "This isn't possible";
            exit(1);
        }
        out << "\n";
    }
    file.close();
}

void AutoClicker::importFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QList<AbstractJob*> jobsTmp = jobs;
    jobs.clear();
    emit jobsChanged();
    foreach (AbstractJob *abstractJob, jobsTmp) {
        delete abstractJob;
    }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList list = line.split(" ",Qt::SkipEmptyParts);
        QVector<int> vector;
        foreach(QString num,list){
            vector.push_back(num.toInt());
        };
        AbstractJob *newJob;
        switch(vector[0]){
            case AbstractJob::TypeTimer:
                newJob = new SleepJob();
                break;
            case AbstractJob::TypeClicker:
                newJob = new ClickerJob();
                ((ClickerJob*)newJob)->setX(vector[3]);
                ((ClickerJob*)newJob)->setY(vector[4]);
                ((ClickerJob*)newJob)->setSide(vector[5]);
                break;
            case AbstractJob::TypeMouseScroll:
                newJob = new MouseWheelJob();
                ((MouseWheelJob*)newJob)->setDirection(vector[3]);
                break;
            default:
                qDebug() << "This isn't possible";
                exit(1);
        }
        newJob->setAmount(vector[1]);
        newJob->setDelay(vector[2]);
        jobs.push_back(newJob);
    }
    emit jobsChanged();
    file.close();

}



QQmlListProperty<AbstractJob> AutoClicker::getJobs()
{
    return {this,&jobs};
}

QString AutoClicker::getGlobalPos()
{
    return globalPos;
}

void AutoClicker::setRepeatAmount(int val)
{
    repeatAmount = val;
    emit repeatAmountChanged();
}

int AutoClicker::getRepeatAmount()
{
    return repeatAmount;
}

QTcpSocket *AutoClicker::getSocket()
{
    return socket;
}

bool AutoClicker::getWorking()
{
    return working;
}



