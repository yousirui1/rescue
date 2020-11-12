#include "qbutton2.h"
#include <QKeyEvent>
#if 0
border-image: url(:/images/bluebutton.png);
color: #FFFFFF;
font: 12pt "黑体";
#endif

QButton2::QButton2(QWidget *parent):QPushButton(parent)
{

}

QButton2::~QButton2()
{

}

void QButton2::focusInEvent(QFocusEvent *e)
{
    QString style = QString("border-image: url(:/images/bluebutton.png);"
                            "color: #FFFFFF;"
                            "font: 12pt '黑体';");
    this->setStyleSheet(style);
}

void QButton2::focusOutEvent(QFocusEvent *e)
{
    this->setStyleSheet("border-image:url(:/images/textEdit.png) 10 10 10 10;"
                        "border-top: 10px transparent;"
                        "border-right: 10px transparent;"
                        "border-bottom: 10px transparent;"
                        "border-left: 10px transparent;"
                        "color: #FFFFFF;"
                        "font: 12pt \"黑体\";");
}
