#include "threadreceive.h"

ThreadReceive::ThreadReceive() :
    m_context(1) ,
    m_socket(m_context, zmq::socket_type::pull)
{

}

ThreadReceive::~ThreadReceive()
{
    m_loop = false;
    msleep(2);
}

bool ThreadReceive::bind(int port)
{
    m_socket.bind("tcp://*:" + std::to_string(port));
    m_connected = m_socket.connected();
    return m_connected;
}

void ThreadReceive::run()
{
    QImage image;
    zmq::message_t mess;

    while (m_loop) {
        if (!m_connected)
            msleep(1);
        else {
            zmq::recv_result_t res = m_socket.recv(mess, zmq::recv_flags::none);
            if (res && res.value() != 0) {
                emit displayCam(QImage::fromData((const uchar *)mess.data(), mess.size(), "JPG"));
            }
            else
                msleep(1);
        }
    }
}
