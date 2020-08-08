#include "globalapplication.h"

GlobalApplication::GlobalApplication(int &argc, char **argv):QApplication (argc, argv)
{
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT());
     timer->start(1000);
}
GlobalApplication::~GlobalApplication()
{
}

bool GlobalApplication::notify(QObject *obj, QEvent *e)
{
    //if(e->type() == QEvent::MouseButtonPress | e->type() == QEvent::TouchBegin | e->type() == QEvent::KeyPress)
    {
        //Opera
    }
    return QApplication::notify(obj,e);
}
