#ifndef PIPETHREAD_H
#define PIPETHREAD_H

#include <QThread>

#define DATA_SIZE 1452
#define HEAD_LEN 8

/* msg */
#define DATA_SYN 0xFF
#define DATA_SYN_OFFSET 0
#define DATA_ENCRYPT 1
#define DATA_ENCRYPT_OFFSET 1
#define DATA_ORDER_OFFSET 2
#define DATA_LEN_OFFSET 4


class PipeThread : public QThread
{
Q_OBJECT
public:
    PipeThread(int pipe_fd[2]);
    void process_pipe(unsigned char *buf, int len);
    int send_pipe(char *buf, short cmd, int size);

protected:
    void run();
private slots:
    void close_pipe();

public:
    int run_flag;
private:
    int pipe_qt[2];
};

#endif // PIPETHREAD_H
