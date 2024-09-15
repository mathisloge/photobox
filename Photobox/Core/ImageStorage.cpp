#include "ImageStorage.hpp"
#include <QDebug>
#include <QImage>
#include <QThread>
#include <qdatetime.h>
using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
namespace
{
class StoreWorkerThread : public QThread
{
    Q_OBJECT
  public:
    StoreWorkerThread(std::filesystem::path storage_dir, QString name, const QImage &image_to_save)
        : storage_dir_{std::move(storage_dir)}
        , name_{std::move(name)}
        , image_to_save_{image_to_save}
    {}

  private:
    void run() override
    {
        const auto image_name = name_ + QStringLiteral(".png");
        qDebug() << "Saving image to " << image_name;
        if (not image_to_save_.save(QString::fromStdString(storage_dir_ / image_name.toStdString())))
        {
            qCritical() << "Could not save image";
        }
    }

  private:
    std::filesystem::path storage_dir_;
    QString name_;
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
        std::filesystem::create_directories(storage_dir_);
    }
}

void ImageStorage::onImageCaptured(const QImage &captured_image)
{
    static const QString kNameTemplate = QStringLiteral("capture_%1");
    StoreWorkerThread *worker_thread =
        new StoreWorkerThread(storage_dir_, kNameTemplate.arg(image_counter_++), captured_image);
    connect(worker_thread, &StoreWorkerThread::finished, worker_thread, &QObject::deleteLater);
    worker_thread->start();
}
} // namespace Pbox
#include "ImageStorage.moc"