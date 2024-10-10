#include "ImageStorage.hpp"
#include <QDebug>
#include <QImage>
#include <QThread>
#include <fmt/format.h>
using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
namespace
{
class StoreWorkerThread : public QThread
{
    Q_OBJECT
  public:
    StoreWorkerThread(std::filesystem::path storage_dir, std::string image_name, const QImage &image_to_save)
        : storage_dir_{std::move(storage_dir)}
        , image_name_{std::move(image_name)}
        , image_to_save_{image_to_save}
    {}
  Q_SIGNALS:
    void imageSaved(const std::filesystem::path &file_path);

  private:
    void run() override
    {
        const auto image_path = storage_dir_ / image_name_;
        qDebug() << "Saving image to " << image_path.c_str();
        if (not image_to_save_.save(QString::fromStdString(image_path)))
        {
            qCritical() << "Could not save image";
            return;
        }
        Q_EMIT imageSaved(image_path);
    }

  private:
    std::filesystem::path storage_dir_;
    std::string image_name_;
    QImage image_to_save_;
};
} // namespace

ImageStorage::ImageStorage(std::filesystem::path storage_dir)
    : storage_dir_{std::move(storage_dir)}
{
    if (std::filesystem::exists(storage_dir_))
    {
        auto current_file_count = 0;
        for (auto entry : std::filesystem::directory_iterator{storage_dir_})
        {
            if (entry.is_regular_file())
            {
                current_file_count++;
            }
        }
        image_counter_ = current_file_count + 1;
    }
    else
    {
        try
        {
            std::filesystem::create_directories(storage_dir_);
            image_counter_ = 1;
        }
        catch (const std::exception &exception)
        {
            qCritical() << exception.what();
        }
    }
}

void ImageStorage::onImageCaptured(const QImage &captured_image)
{
    StoreWorkerThread *worker_thread = new StoreWorkerThread(storage_dir_, generateNewImageFilePath(), captured_image);
    connect(worker_thread, &StoreWorkerThread::imageSaved, this, &ImageStorage::imageSaved);
    connect(worker_thread, &StoreWorkerThread::finished, worker_thread, &QObject::deleteLater);
    worker_thread->start();
}

std::string ImageStorage::generateNewImageFilePath()
{
    constexpr std::string_view kNameTemplate = "capture";
    return fmt::format("{}_{}.png", kNameTemplate, image_counter_++);
}

const std::filesystem::path &ImageStorage::storageDir() const
{
    return storage_dir_;
}

} // namespace Pbox
#include "ImageStorage.moc"