#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QStackedLayout>
#include <QWidget>
#include "uploadwindow.h"
#include "configwindow.h"
#include "dialogwindow.h"
#include "oswindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);
    //void focusInEvent( QFocusEvent * event);
    //bool eventFilter(QObject *watched, QEvent *event);

signals:
    void display(int number);

private slots:
    void on_installButton_clicked();

    void on_uploadButton_clicked();

    void on_rebootButton_clicked();

private:
    Ui::MainWindow *ui;
    Uploadwindow *upload_ui;
    ConfigWindow *config_ui;
    OSWindow *os_ui;
    QStackedLayout *stackLayout;
    QVBoxLayout *mainLayout;
public:
    QLabel *date_label;
    QLabel *ip_label;
    QLabel *netstate_label;
    QLabel *neticon_label;
    QLabel *netpoint_label;
    DialogWindow *dialog_ui;
};

#endif // MAINWINDOW_H
