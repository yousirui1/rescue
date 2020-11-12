#ifndef QBUTTON2_H
#define QBUTTON2_H

#include <QPushButton>
class QButton2 : public QPushButton
{
Q_OBJECT
public:
      QButton2(QWidget *parent=0);
      ~QButton2();
protected:
      virtual void focusInEvent(QFocusEvent *e);
      virtual void focusOutEvent(QFocusEvent *e);
};


#endif // QBUTTON2_H
