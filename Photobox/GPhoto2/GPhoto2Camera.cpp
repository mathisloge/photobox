#include "GPhoto2Camera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QThread>
#include <QVideoFrameFormat>
#include <fcntl.h>
#include "GPhoto2Worker.hpp"
#include <gphoto2-abilities-list.h>
#include <gphoto2-camera.h>
namespace Pbox
{
GPhoto2Camera::GPhoto2Camera()
{
    connect(&worker_thread_, &GPhoto2Worker::gotPreviewImage, this, [this](QImage image) {
        QPointer<QVideoSink> video_sink{getVideoSink()};

        if (video_sink.isNull())
        {
            return;
        }
        QVideoFrame video_frame = QVideoFrame{QVideoFrameFormat{image.size(), QVideoFrameFormat::Format_RGBA8888}};
        if (video_frame.map(QVideoFrame::WriteOnly))
        {
            // Zugriff auf die Rohdaten des Frames (erste Plane, da RGB nur eine Plane hat)
            uchar *data = video_frame.bits(0); // Zugriff auf Plane 0 für RGB32

            if (data != nullptr)
            {
                // Berechne die Größe der Frame-Daten
                const int bytesPerLine = video_frame.bytesPerLine(0);
                const int frameHeight = video_frame.height();
                const int bufferSize = bytesPerLine * frameHeight;

                image = image.convertedTo(QImage::Format::Format_RGBA8888);
                std::copy(image.bits(), image.bits() + image.sizeInBytes(), data);
            }
            else
            {
                qDebug() << "Fehler beim Zugriff auf die Frame-Daten.";
            }

            video_frame.unmap(); // Immer unmap aufrufen, wenn die Bearbeitung abgeschlossen ist
        }
        else
        {
            qDebug() << "Fehler beim Mappen des QVideoFrame.";
        }

        video_sink->setVideoFrame(video_frame);
    });
    connect(
        &worker_thread_, &GPhoto2Worker::gotCapturedImage, this, [this](QImage image) { Q_EMIT imageCaptured(image); });

    worker_thread_.start();
}

GPhoto2Camera::~GPhoto2Camera()
{
    worker_thread_.requestStop();
    worker_thread_.wait();
};

void GPhoto2Camera::requestCapturePhoto()
{
    worker_thread_.requestCapture();
}
} // namespace Pbox
