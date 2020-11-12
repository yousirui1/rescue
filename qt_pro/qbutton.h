#ifndef MPUSHBUTTON_H
#define MPUSHBUTTON_H

#include <QPushButton>
class QButton : public QPushButton
{
Q_OBJECT
public:
      QButton(QWidget *parent=0);
      ~QButton();
protected:
      virtual void focusInEvent(QFocusEvent *e);
      virtual void focusOutEvent(QFocusEvent *e);
};


#endif // MPUSHBUTTON_H
