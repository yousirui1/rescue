#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "dialogwindow.h"

#include <QButtonGroup>
#include <QLabel>
#include <QWidget>

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = NULL);
    ~ConfigWindow();
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);

signals:
    void display(int number);

private slots:
    void on_laststepButton_clicked();
    void on_nextButton_clicked();
    void on_formatButton_clicked();
    void slots_dhcpable();
    void install_cacenl_clicked();
    void install_confirm_btn();
    void on_num_edit_textChanged(const QString &arg1);


private:
    Ui::ConfigWindow *ui;
    QButtonGroup *group_btn;
    //int dhcp_flag;
    bool auto_cloud_flag;
public:
    QLabel *date_label;
    DialogWindow *dialog_ui;
};


#endif // CONFIGWINDOW_H
