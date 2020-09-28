#include "mpushbutton.h"
#include "global.h"


mPushButton::mPushButton(QWidget *parent):QPushButton(parent)
{

}

mPushButton::~mPushButton()
{

}

void mPushButton::focusInEvent(QFocusEvent *e)
{
    //DEBUG("focusInEvent %s", this->objectName().toStdString().c_str());
    //this->setFocus();
    //this->setShortcut( QKeySequence::InsertParagraphSeparator);
    //this->setShortcut(Qt::Key_Enter);
    //this->setShortcut(Qt::Key_Return);

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

void mPushButton::focusOutEvent(QFocusEvent *e)
{

    this->setStyleSheet("font: 12pt \"黑体\";"
                        "border-image:url(:/images/nice_light.pngg) 10 10 10 10;"
                        "border-top: 10px transparent;"
                        "border-right: 10px transparent;"
                        "border-bottom: 10px transparent;"
                        "border-left: 10px transparent;"
                        "color: #FFFFFF;");

}


#if 0
    if(STRPREFIX(current_focus->objectName().toStdString().c_str(), "installButton"))
    {
        DEBUG("installButton ------------");
        ui->uploadButton->clearFocus();
        ui->rebootButton->clearFocus();
        ui->installButton->setFocus();    //设置默认焦点
        ui->installButton->setShortcut( QKeySequence::InsertParagraphSeparator );  //设置快捷键为键盘的“回车”键
        ui->installButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键
        ui->installButton->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    }
    if(STRPREFIX(current_focus->objectName().toStdString().c_str(), "uploadButton"))
    {
        DEBUG("uploadButton ------------");
        ui->installButton->clearFocus();
        ui->rebootButton->clearFocus();
        ui->uploadButton->setFocus();    //设置默认焦点
        ui->uploadButton->setShortcut( QKeySequence::InsertParagraphSeparator );  //设置快捷键为键盘的“回车”键
        ui->uploadButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键
        ui->uploadButton->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    }
    if(STRPREFIX(current_focus->objectName().toStdString().c_str(), "rebootButton"))
    {
        DEBUG("rebootButton ------------");
        ui->uploadButton->clearFocus();
        ui->installButton   ->clearFocus();
        ui->rebootButton->setFocus();    //设置默认焦点
        ui->rebootButton->setShortcut( QKeySequence::InsertParagraphSeparator );  //设置快捷键为键盘的“回车”键
        ui->rebootButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键
        ui->rebootButton->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    }
#endif


#if 0
    QPushButton *btn = qobject_cast<QPushButton*>(current_focus);
    if(btn)
    {
        DEBUG("btn->name %s", btn->objectName().toStdString().c_str());
        btn->setFocus();
        btn->setShortcut( QKeySequence::InsertParagraphSeparator); //设置快捷键为键盘的“回车”键
        btn->setShortcut(Qt::Key_Enter);   //设置快捷键为enter键
        btn->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    }
#endif
