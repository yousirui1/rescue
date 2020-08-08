#ifndef OSWINDOW_H
#define OSWINDOW_H

#include "dialogwindow.h"

#include <QLabel>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class OSWindow;
}

class OSWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OSWindow(QWidget *parent = NULL);
    void keyPressEvent(QKeyEvent *event);
    ~OSWindow();

signals:
    void display(int number);

private:
    Ui::OSWindow *ui;
public:
    QLabel *date_label;
    QLabel *ip_label;
    QLabel *netstate_label;
    QLabel *neticon_label;
    QLabel *netpoint_label;
    DialogWindow *dialog_ui;
private slots:
    void on_pushButton_12_clicked();
    void on_pushButton_11_clicked();
    void on_template_size_edit_textChanged(const QString &arg1);


};

#endif // OSWINDOW_H
