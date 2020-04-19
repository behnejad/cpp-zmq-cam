#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    showMaximized();
    connect(&m_preview, SIGNAL(previewImage(const QImage &)), this, SLOT(previewCam(const QImage &)));
    connect(&m_receive, SIGNAL(displayCam(const QImage &)), this, SLOT(displayCam(const QImage &)));
    m_preview.start();
    m_receive.start();
}

Dialog::~Dialog()
{
    m_receive.terminate();
    m_preview.terminate();
    delete ui;
}

void Dialog::previewCam(const QImage & img)
{
    ui->preview->setPixmap(QPixmap::fromImage(img.scaled(ui->preview->size(), Qt::KeepAspectRatio)));
}

void Dialog::displayCam(const QImage & img)
{
    ui->display->setPixmap(QPixmap::fromImage(img.scaled(ui->display->size(), Qt::KeepAspectRatio)));
}

void Dialog::on_capture_clicked()
{
    if (m_capture) {
        m_preview.stopPreview();
        ui->capture->setText("Capture");
        m_capture = false;
    }
    else {
        m_preview.startPreview();
        ui->capture->setText("End");
        m_capture = true;
    }
}

void Dialog::on_connect_clicked()
{
    m_preview.connect(ui->to_address->text(), ui->to_port->text().toInt());
    ui->connect->setDisabled(true);
}

void Dialog::on_bind_clicked()
{
    m_receive.bind(ui->bind_port->text().toInt());
    ui->bind->setDisabled(true);
}
