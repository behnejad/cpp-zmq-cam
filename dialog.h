#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "threadpreview.h"
#include "threadreceive.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
    void previewCam(const QImage & img);
    void displayCam(const QImage & img);

private slots:
    void on_capture_clicked();

    void on_connect_clicked();

    void on_bind_clicked();

private:
    Ui::Dialog *ui;
    ThreadPreview m_preview;
    ThreadReceive m_receive;

    bool m_capture = false;
};
#endif // DIALOG_H
