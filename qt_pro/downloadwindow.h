#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include "QRoundProgressBar.h"
#include "dialogwindow.h"

#include <QLabel>
#include <QWidget>

namespace Ui {
class DownloadWindow;
}

class DownloadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWindow(QWidget *parent = NULL);
    ~DownloadWindow();
     void keyPressEvent(QKeyEvent *e);

public slots:
    void setProgressValue(struct progress_info *info);

signals:
   void display(int number);

private:
    Ui::DownloadWindow *ui;


public:
   QLabel *date_label;
   QLabel *ip_label;
   QLabel *netstate_label;
   QLabel *neticon_label;
   QLabel *netpoint_label;
   DialogWindow *dialog_ui;
   QRoundProgressBar *progress;
};

#endif // DOWNLOADWINDOW_H
