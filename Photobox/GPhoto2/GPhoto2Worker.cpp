#include "GPhoto2Worker.hpp"
#include <QDebug>
#include <QImage>

namespace Pbox
{
using CameraFilePtr = std::unique_ptr<CameraFile, decltype(&gp_file_free)>;
namespace
{
CameraFilePtr make_camera_file();
std::optional<QImage> readImageFromFile(CameraFile *file);
} // namespace

void GPhoto2Worker::requestStop()
{
    running_ = false;
}

void GPhoto2Worker::requestCapture()
{
    capture_requested_ = true;
}

void GPhoto2Worker::run()
{
    while (running_)
    {
        if (camera_.camera() == nullptr)
        {
            const auto success = camera_.autodetectAndConnectCamera();
            if (success)
            {
                qDebug() << "Got GPhoto2 camera";
            }
        }
        else
        {
            run_normal_operation();
        }
    }
}

void GPhoto2Worker::run_normal_operation()
{
    auto file = make_camera_file();
    Q_ASSERT(file != nullptr);

    CameraFilePath camera_file_path;

    /* NOP: This gets overridden in the library to /capt0000.jpg */
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "foo.jpg");
    while (camera_.camera() != nullptr and running_)
    {
        if (capture_requested_)
        {
            // gp_camera_trigger_capture(camera_.camera(), camera_.context()) <
            const auto ret_val =
                gp_camera_capture(camera_.camera(), GP_CAPTURE_IMAGE, &camera_file_path, camera_.context());
            if (ret_val < GP_OK)
            {
                qDebug() << "Could not trigger capture";
            }
            else
            {
                capture_requested_ = false;
                auto captured_image = readImageFromFile(file.get());
                if (captured_image.has_value())
                {
                    Q_EMIT gotCapturedImage(captured_image.value());
                }
            }
        }

        const auto ret_val = gp_camera_capture_preview(camera_.camera(), file.get(), camera_.context());
        if (ret_val < GP_OK)
        {
            qDebug() << "could not capture preview input";
            continue;
        }
        auto preview_image = readImageFromFile(file.get());
        if (preview_image.has_value())
        {
            Q_EMIT gotPreviewImage(preview_image.value());
        }
        // run_event_queue();
    }
}

void GPhoto2Worker::run_event_queue()
{
    Q_ASSERT(camera_.camera());
    while (running_)
    {
        CameraEventType evttype{};
        void *evtdata{nullptr};
        const auto retval = gp_camera_wait_for_event(camera_.camera(), 1, &evttype, &evtdata, camera_.context());
        if (retval != GP_OK)
        {
            break;
        }

        switch (evttype)
        {
        case GP_EVENT_FILE_ADDED:
            qDebug() << "Got event GP_EVENT_FILE_ADDED";
            break;
        case GP_EVENT_FILE_CHANGED:
            qDebug() << "Got event GP_EVENT_FILE_CHANGED";
            break;
        case GP_EVENT_FOLDER_ADDED:
            qDebug() << "Got event GP_EVENT_FOLDER_ADDED";
            break;
        case GP_EVENT_CAPTURE_COMPLETE:
            qDebug() << "Got event GP_EVENT_CAPTURE_COMPLETE";
            break;
        case GP_EVENT_TIMEOUT:
            qDebug() << "Got event GP_EVENT_TIMEOUT";
            break;
        case GP_EVENT_UNKNOWN:
            qDebug() << "Got event GP_EVENT_UNKNOWN";
            break;
        default:
            qDebug() << "Got unexpected event =" << evttype;
            break;
        }
        if (evtdata != nullptr)
        {
            free(evtdata);
        }
    }
}

namespace
{
CameraFilePtr make_camera_file()
{
    CameraFile *file{nullptr};
    if (gp_file_new(&file) < GP_OK)
    {
        qCritical() << "Could not allocate camera file";
        return {nullptr, gp_file_free};
    }
    return {file, gp_file_free};
}

std::optional<QImage> readImageFromFile(CameraFile *file)
{
    Q_ASSERT(file != nullptr);

    const char *buffer{};
    unsigned long int size{};
    const auto size_result = gp_file_get_data_and_size(file, &buffer, &size);
    if (size_result < GP_OK)
    {
        qDebug() << "could not get size of file" << size_result;
        return std::nullopt;
    }
    QImage img;
    if (!img.loadFromData(reinterpret_cast<const uchar *>(buffer), static_cast<int>(size)))
    {
        qDebug() << "Fehler beim Laden des Bildes aus den Daten";
        return std::nullopt;
    }
    return img;
}
} // namespace

} // namespace Pbox