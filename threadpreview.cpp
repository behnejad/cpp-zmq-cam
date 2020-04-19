#include "threadpreview.h"

ThreadPreview::ThreadPreview() :
    m_context(1) ,
    m_socket(m_context, zmq::socket_type::push)
{

}

ThreadPreview::~ThreadPreview()
{
    m_loop = false;
    msleep(2);
    if (m_cam_open)
        m_camera.release();
}

void ThreadPreview::startPreview()
{
    m_preview = true;
}

void ThreadPreview::stopPreview()
{
    m_preview = false;
}

void ThreadPreview::connect(QString ip, int port)
{
    m_socket.connect("tcp://" + ip.toStdString() + ":" + std::to_string(port));
    m_send_image = m_socket.connected();
}

void ThreadPreview::disconnect()
{
    m_send_image = false;
}

void ThreadPreview::run()
{
    Mat frame;
    QImage image;
    bool init = false;
    QByteArray data;
    QBuffer buffer(&data);

    while (m_loop) {
        if (m_preview) {
            if (!m_cam_open) {
                m_camera.open(0);
                m_camera.set(CAP_PROP_FRAME_WIDTH,640);
                m_camera.set(CAP_PROP_FRAME_HEIGHT,480);
                if (!m_camera.isOpened())
                    qDebug() << "Camera open error";
                else
                    m_cam_open = true;
            }

            if (m_cam_open) {
                m_camera >> frame;
                cvtColor(frame, frame, COLOR_BGR2RGB);
                flip(frame, frame, +1);
                if (!init) {
                    init = true;
                    image = QImage(frame.data, frame.cols, frame.rows, frame.step,
                                           QImage::Format_RGB888, nullptr);
                }
                emit previewImage(image);
            }
        }
        else {
            if (m_cam_open) {
                m_camera.release();
                m_cam_open = false;
            }
        }

        if (m_send_image && m_cam_open && m_preview) {
            data.clear();
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "JPG");
            m_socket.send(zmq::message_t(data.data(), data.length()), zmq::send_flags::none);
            buffer.close();
        }

        if (!m_preview && !m_cam_open)
            msleep(1);
    }
}
