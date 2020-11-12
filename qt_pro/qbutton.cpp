#include "qbutton.h"
#include "global.h"
#include <QKeyEvent>


QButton::QButton(QWidget *parent):QPushButton(parent)
{

}

QButton::~QButton()
{

}

void QButton::focusInEvent(QFocusEvent *e)
{
    QString style = QString("font: 12pt \"黑体\";"
                    "border-image:url(:/images/nice_black.png) 10 10 10 10;"
                    "border-top: 10px transparent;"
                    "border-right: 10px transparent;"
                    "border-bottom: 10px transparent;"
                    "border-left: 10px transparent;"
                    "color: #FFFFFF;"

                    "QPushButton#%1:pressed"
                    "{"
                     "       border-image:url(:/images/nice_black.png) 10 10 10 10;"
                           " border-top: 10px transparent;"
                          "  border-right: 10px transparent;"
                         "   border-bottom: 10px transparent;"
                           " border-left: 10px transparent;"
                    "}").arg(this->objectName().toStdString().c_str());
    this->setStyleSheet(style);
}

void QButton::focusOutEvent(QFocusEvent *e)
{

    this->setStyleSheet("font: 12pt \"黑体\";"
                        "border-image:url(:/images/nice_light.pngg) 10 10 10 10;"
                        "border-top: 10px transparent;"
                        "border-right: 10px transparent;"
                        "border-bottom: 10px transparent;"
                        "border-left: 10px transparent;"
                        "color: #FFFFFF;");

}
