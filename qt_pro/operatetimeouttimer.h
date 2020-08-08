#ifndef OPERATETIMEOUTTIMER_H
#define OPERATETIMEOUTTIMER_H

#include <QTimer>
#include <QObject>
#include <QList>

class OperateTimeOutTimer : public QObject
{
   Q_OBJECT
public:
    static OperateTimeOutTimer* getOperateTimeOutTimer();
    void setHomePageFlag(bool flag);

private:
    OperateTimeOutTimer();

signals:
    void timeOutSignal(int timeOutMinute);
public slots:
    void on_timeOut();
    void on_resetTimeOutSecond();

private:
    static OperateTimeOutTimer *mOperateTimeOutTimer;
    QTimer *mSecondTimer;
    int mTimeoutSecond;
    bool mHomePageFlag;
};

#endif // OPERATETIMEOUTTIMER_H
