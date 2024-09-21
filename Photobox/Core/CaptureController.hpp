#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QtQmlIntegration>

namespace Pbox
{
class ICamera;
class ImageStorage;
class CollageRenderer;
class CameraImageProvider;

class CaptureImageModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    enum class Roles
    {
        ImageSource = Qt::DisplayRole + 1,
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetImageCount(int count);
    void setImage(int index, QString source);

  private:
    std::vector<QString> image_sources_;
};

class CaptureController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CaptureImageModel *model READ getModel CONSTANT)

  public:
    explicit CaptureController(std::unique_ptr<ImageStorage> image_storage, std::shared_ptr<ICamera> camera);
    ~CaptureController() override;
    Q_DISABLE_COPY_MOVE(CaptureController);

    Q_INVOKABLE void captureImage();
    CaptureImageModel *getModel();
    CameraImageProvider *createImageProvider();

  Q_SIGNALS:
    void collageCaptureComplete();
    void imageCaptured(const QImage &image, const QString &image_id);

  private:
    std::unique_ptr<ImageStorage> image_storage_;
    std::shared_ptr<ICamera> camera_;
    std::unique_ptr<CollageRenderer> collage_renderer_;
    CaptureImageModel capture_model_;
    int current_image_count_{0};
    int image_count_{0};
    int image_count_collage_;

    // a counter to invalidate any image providers
    std::uint32_t image_counter_{0};
};
} // namespace Pbox