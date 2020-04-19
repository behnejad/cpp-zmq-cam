#ifndef THREADRECEIVE_H
#define THREADRECEIVE_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>

#include <zmq.hpp>

class ThreadReceive : public QThread
{
    Q_OBJECT
public:
    ThreadReceive();
    virtual ~ThreadReceive();
    bool bind(int port);
    void run() override;

private:
    bool m_loop = true;
    bool m_connected = false;
    zmq::context_t m_context;
    zmq::socket_t m_socket;

signals:
    void displayCam(const QImage &);
};

#endif // THREADRECEIVE_H
