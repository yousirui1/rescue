#ifndef GLOBALAPPLICATION_H
#define GLOBALAPPLICATION_H

#include <QApplication>
#include <QTimer>

class GlobalApplication :QApplication
{
public:
    GlobalApplication(int &argc, char **argv);
    ~GlobalApplication();
    bool notify(QObject *, QEvent *);
};

#endif // GLOBALAPPLICATION_H
