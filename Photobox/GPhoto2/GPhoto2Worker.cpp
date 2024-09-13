#include "GPhoto2Worker.hpp"
#include <QDateTime>
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

    int error_count{0};
    while (camera_.camera() != nullptr and running_)
    {
        if (capture_requested_)
        {
            qDebug() << "STart capture" << QDateTime::currentDateTime();
            // const auto ret_val = gp_camera_trigger_capture(camera_.camera(), camera_.context());
            const auto ret_val =
                gp_camera_capture(camera_.camera(), GP_CAPTURE_IMAGE, &camera_file_path, camera_.context());
            if (ret_val < GP_OK)
            {
                qDebug() << "Could not trigger capture" << QDateTime::currentDateTime();
            }
            else
            {
                capture_requested_ = false;
                qDebug() << "start get photo" << QDateTime::currentDateTime();
                gp_camera_file_get(camera_.camera(),
                                   camera_file_path.folder,
                                   camera_file_path.name,
                                   GP_FILE_TYPE_NORMAL,
                                   file.get(),
                                   camera_.context());

                qDebug() << "start readImageFromFile" << QDateTime::currentDateTime();

                auto captured_image = readImageFromFile(file.get());
                if (captured_image.has_value())
                {
                    qDebug() << "start gotCapturedImage" << QDateTime::currentDateTime();
                    Q_EMIT gotCapturedImage(captured_image.value());
                }
                qDebug() << "end capture" << QDateTime::currentDateTime();
            }
        }
        const auto ret_val = gp_camera_capture_preview(camera_.camera(), file.get(), camera_.context());
        if (ret_val < GP_OK)
        {
            error_count++;
            if (error_count > 5)
            {
                qCritical() << "couldn't capture image multiple times. Leaving capture loop now";
                camera_.closeCamera();
                break;
            }
            qDebug() << "could not capture preview input";
            continue;
        }
        auto preview_image = readImageFromFile(file.get());
        if (preview_image.has_value())
        {
            error_count = 0;
            Q_EMIT gotPreviewImage(preview_image.value());
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