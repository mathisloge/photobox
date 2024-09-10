#include "GPhoto2Camera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QThread>
#include <QVideoFrameFormat>
#include <fcntl.h>
#include <gphoto2-abilities-list.h>
#include <gphoto2-camera.h>
namespace Pbox
{

class GPhoto2WorkerThread : public QThread
{
    Q_OBJECT
  public:
    ~GPhoto2WorkerThread() override
    {
        qDebug() << "destroy gphoto2";
    }

  Q_SIGNALS:
    void gotPreviewImage(QImage image);

  private:
    void run() override
    {
        qDebug() << "start gphoto2";
        CameraList *list{nullptr};
        if (gp_list_new(&list) < GP_OK)
        {
            qDebug() << "could not allocate list";
            return;
        }

        GPContext *context = gp_context_new();
        gp_context_set_error_func(
            context,
            [](GPContext *context, const char *str, void *data) { qDebug() << "gphoto2 error:" << str; },
            NULL);
        gp_context_set_status_func(
            context,
            [](GPContext *context, const char *str, void *data) { qDebug() << "gphoto2 status:" << str; },
            NULL);

        const auto auto_detect_count = gp_camera_autodetect(list, context);
        if (auto_detect_count <= GP_OK)
        {
            qDebug() << "got no possible cameras";
            return;
        }

        qDebug() << "auto connect result" << auto_detect_count;
        const char *model_name;
        const char *port_value;
        gp_list_get_name(list, 0, &model_name);
        gp_list_get_value(list, 0, &port_value);
        qDebug() << "model_name=" << model_name << port_value;

        CameraAbilitiesList *abilities{nullptr};
        gp_abilities_list_new(&abilities);
        gp_abilities_list_load(abilities, context);

        Camera *camera{nullptr};
        if (gp_camera_new(&camera) < GP_OK)
        {
            qDebug() << "could not allocate camera";
            return;
        }

        const auto camera_index = gp_abilities_list_lookup_model(abilities, model_name);
        CameraAbilities camera_abilities;
        gp_abilities_list_get_abilities(abilities, camera_index, &camera_abilities);
        gp_camera_set_abilities(camera, camera_abilities);

        GPPortInfoList *port_info_list{nullptr};
        /* Load all the port drivers we have... */
        gp_port_info_list_new(&port_info_list);
        gp_port_info_list_load(port_info_list);
        gp_port_info_list_count(port_info_list);

        const auto port_lookup_result = gp_port_info_list_lookup_path(port_info_list, port_value);
        switch (port_lookup_result)
        {
        case GP_ERROR_UNKNOWN_PORT:
            qCritical() << "The port you specified "
                           "('"
                        << port_value
                        << "') can not be found. Please "
                           "specify one of the ports found by "
                           "'gphoto2 --list-ports' and make "
                           "sure the spelling is correct "
                           "(i.e. with prefix 'serial:' or 'usb:').";
            break;
        default:
            break;
        }
        if (port_lookup_result < GP_OK)
        {
            return;
        }
        GPPortInfo port_info{};
        gp_port_info_list_get_info(port_info_list, port_lookup_result, &port_info);

        gp_camera_set_port_info(camera, port_info);

        CameraFile *file{nullptr};
        gp_file_new(&file);
        while (true)
        {
            const auto ret_val = gp_camera_capture_preview(camera, file, context);
            if (ret_val < GP_OK)
            {
                qDebug() << "could not capture input";
            }
            else
            {
                const char *buffer{};
                unsigned long int size{};
                const auto size_result = gp_file_get_data_and_size(file, &buffer, &size);
                if (size_result < GP_OK)
                {
                    qDebug() << "could not get size of file" << size_result;
                    continue;
                }
                QImage img;
                if (!img.loadFromData(reinterpret_cast<const uchar*>(buffer), static_cast<int>(size)))
                {
                    qDebug() << "Fehler beim Laden des Bildes aus den Daten";
                    continue;
                }
                Q_EMIT gotPreviewImage(img);
            }
        }
    }
};

GPhoto2Camera::GPhoto2Camera()
{
    GPhoto2WorkerThread *workerThread = new GPhoto2WorkerThread();

    connect(workerThread, &GPhoto2WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
    connect(workerThread, &GPhoto2WorkerThread::gotPreviewImage, this, [this](QImage image) {
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

                std::copy(image.bits(), image.bits() + image.sizeInBytes(), data);

                qDebug() << "Daten erfolgreich geschrieben!";
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

#include "GPhoto2Camera.moc"