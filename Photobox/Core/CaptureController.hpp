#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QtQmlIntegration>
#include <exec/async_scope.hpp>
#include "CollageFontCache.hpp"
#include "CollageSettings.hpp"
#include "Scheduler.hpp"
namespace Pbox
{
class ICamera;
class ImageStorage;
class CollageRenderer;
class CameraImageProvider;
class CollagePrinter;

class CaptureImageModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    enum class Roles
    {
        ImageSource = Qt::DisplayRole + 1,
    };

    Q_INVOKABLE QString sourceOfLastItem() const;

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
    Q_PROPERTY(bool collageComplete READ isCollageComplete NOTIFY collageCompletedChanged)
    Q_PROPERTY(QString collageImagePath READ getCollageImagePath NOTIFY collageCaptureComplete);

  public:
    explicit CaptureController(Scheduler &scheduler,
                               const std::filesystem::path &collage_directory,
                               std::unique_ptr<ImageStorage> image_storage,
                               std::shared_ptr<ICamera> camera,
                               std::unique_ptr<CollagePrinter> printer);
    ~CaptureController() override;
    Q_DISABLE_COPY_MOVE(CaptureController);

    Q_INVOKABLE void captureImage();
    Q_INVOKABLE void reset();
    CaptureImageModel *getModel();
    CameraImageProvider *createImageProvider();
    bool isCollageComplete() const;
    QString getCollageImagePath();

  private:
    void loadSettings(const std::filesystem::path &collage_directory);
    void saveImage(const QImage &image);

  Q_SIGNALS:
    void collageCompletedChanged();
    void collageCaptureComplete();
    void imageCaptured(const QImage &image, const QString &image_id);
    void capturedImageReady();

  private:
    Scheduler &scheduler_;
    exec::async_scope async_scope_;
    CollageFontCache font_cache_;
    std::unique_ptr<ImageStorage> image_storage_;
    std::shared_ptr<ICamera> camera_;
    std::unique_ptr<CollagePrinter> printer_;
    std::unique_ptr<CollageRenderer> collage_renderer_;
    CaptureImageModel capture_model_;
    CollageSettings settings_;
    int current_capture_{0};
    int max_capture_count_{0};
    bool collage_finished_{false};
    std::filesystem::path collage_image_path_;

    // a counter to invalidate any image providers
    std::uint32_t image_counter_{0};
};
} // namespace Pbox
