#ifndef THREADPREVIEW_H
#define THREADPREVIEW_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>

#include <zmq.hpp>

#include <opencv2/opencv.hpp>
using namespace cv;

class ThreadPreview : public QThread
{
    Q_OBJECT
public:
    ThreadPreview();
    void startPreview();
    void stopPreview();
    void connect(QString ip, int port);
    void disconnect();
    void run() override;
    virtual ~ThreadPreview();

private:
    bool m_preview = false;
    bool m_send_image = false;
    bool m_loop = true;
    bool m_cam_open = false;

    zmq::context_t m_context;
    zmq::socket_t m_socket;
    VideoCapture m_camera;

signals:
    void previewImage(const QImage &);
};

#endif // THREADPREVIEW_H
