#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class DialogWindow;
}

typedef enum DIALOG_TYPE{
    DIALOG_NET = 0,
    DIALOG_INSTALL,
    DIALOG_UNINSTALL,
    DIALOG_ERROR,
}DIALOG_TYPE;

class DialogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DialogWindow(QWidget *parent = NULL);
    ~DialogWindow();
    void setType(int);
    void hideAll();

    void showEvent(QShowEvent *e);
    void keyPressEvent(QKeyEvent *e);

signals:
    void display(int number);

public slots:
    void setErrorText(QString msg);

private slots:
    void on_cacel_btn_clicked();
    void slots_dhcpable();
    void on_confirm_btn_clicked();

    void on_install_cacel_btn_clicked();

    void on_install_confirm_btn_clicked();

    void on_error_confirm_btn_clicked();

private:
    Ui::DialogWindow *ui;
    QButtonGroup *group_btn;
    int dhcp_flag;
public:
    QPushButton  *install_cacel_btn;
    QPushButton  *install_confirm_btn;
};

#endif // DIALOGWINDOW_H
