#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "global.h"
#include <QWSServer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Global *gloab = Global::getGlobal();
    //#0347a8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Global *global = Global::getGlobal();

    global->setWindowFlag(1);
    int pipe[2] = {0,0};
    global->setPipe(pipe);

    MainWindow w;
    w.show();

    return a.exec();
}
