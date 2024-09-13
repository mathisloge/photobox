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
    StoreWorkerThread(QString name, const QImage &image_to_save)
        : name_{std::move(name)}
        , image_to_save_{image_to_save}
    {}

  private:
    void run() override
    {
        const auto image_name = name_ + QStringLiteral(".png");
        qDebug() << "Saving image to " << image_name;
        if (not image_to_save_.save(image_name))
        {
            qCritical() << "Could not save image";
        }
    }

  private:
    QString name_;
    QImage image_to_save_;
};
} // namespace

ImageStorage::ImageStorage()
{}

void ImageStorage::onImageCaptured(const QImage &captured_image)
{
    static const QString kNameTemplate = QStringLiteral("capture_%1");
    StoreWorkerThread *worker_thread = new StoreWorkerThread(kNameTemplate.arg(image_counter_++), captured_image);
    connect(worker_thread, &StoreWorkerThread::finished, worker_thread, &QObject::deleteLater);
    worker_thread->start();
}
} // namespace Pbox
#include "ImageStorage.moc"