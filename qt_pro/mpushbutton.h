#ifndef MPUSHBUTTON_H
#define MPUSHBUTTON_H

#include <QPushButton>
class mPushButton : public QPushButton
{
Q_OBJECT
public:
      mPushButton(QWidget *parent=0);
      ~mPushButton();
protected:
      virtual void focusInEvent(QFocusEvent *e);
      virtual void focusOutEvent(QFocusEvent *e);
};


#endif // MPUSHBUTTON_H
