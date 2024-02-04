#include "inputevent.h"
#include "qcoreapplication.h"
int main(int argc, char *argv[]){
    qDebug() << "started";
    QCoreApplication a(argc, argv);
    InputEvent *inputEvent = new InputEvent();
    a.exec();


}
