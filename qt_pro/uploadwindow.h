#ifndef UPLOADWINDOW_H
#define UPLOADWINDOW_H

#include <QLabel>
#include <QWidget>
#include "dialogwindow.h"
#include "QRoundProgressBar.h"

namespace Ui {
class Uploadwindow;
}

class Uploadwindow : public QWidget
{
    Q_OBJECT

public:
    explicit Uploadwindow(QWidget *parent = NULL);
    ~Uploadwindow();
     void keyPressEvent(QKeyEvent *e);
     void showEvent(QShowEvent *e);
     void upConfig(struct config *conf);

public slots:
     void setProgressValue(struct progress_info *info);
     void showErrorDialog(char *err_msg);
     void Reboot();

signals:
    void display(int number);

private:
    Ui::Uploadwindow *ui;

public:
    QLabel *date_label;
    QLabel *ip_label;
    QLabel *netstate_label;
    QLabel *neticon_label;
    QLabel *netpoint_label;
    DialogWindow *dialog_ui;
    QRoundProgressBar *progress;
};

#endif // UPLOADWINDOW_H
