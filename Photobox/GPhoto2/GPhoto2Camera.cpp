#include "GPhoto2Camera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QVideoFrameFormat>

namespace Pbox
{
GPhoto2Camera::GPhoto2Camera()
{
    /**
    frame_{QVideoFrameFormat{QSize{640, 480}, QVideoFrameFormat::Format_RGBA8888}};

    QPointer<QVideoSink> video_sink{getVideoSink()};
    if (video_sink.isNull())
    {
        return;
    }
    video_sink->setVideoFrame(frame_);

    // Frame in den Speicherbereich mappen, um Daten zu schreiben
    if (frame_.map(QVideoFrame::WriteOnly))
    {
        // Zugriff auf die Rohdaten des Frames (erste Plane, da RGB nur eine Plane hat)
        uchar *data = frame_.bits(0); // Zugriff auf Plane 0 für RGB32

        if (data)
        {
            // Berechne die Größe der Frame-Daten
            int bytesPerLine = frame_.bytesPerLine(0);
            int frameHeight = frame_.height();
            int bufferSize = bytesPerLine * frameHeight;

            QImage x{QSize{640, 480}, QImage::Format::Format_RGBA8888};
            x.fill(Qt::red);

            std::copy(x.bits(), x.bits() + x.sizeInBytes(), data);

            qDebug() << "Daten erfolgreich geschrieben!";
        }
        else
        {
            qDebug() << "Fehler beim Zugriff auf die Frame-Daten.";
        }

        frame_.unmap(); // Immer unmap aufrufen, wenn die Bearbeitung abgeschlossen ist
    }
    else
    {
        qDebug() << "Fehler beim Mappen des QVideoFrame.";
    }
    */
}

GPhoto2Camera::~GPhoto2Camera() = default;

void GPhoto2Camera::requestCapturePhoto()
{}
} // namespace Pbox