#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QtQmlIntegration>

namespace Pbox
{
class ICamera;

class CaptureImageModel final : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    enum class Roles
    {
        ImageSource = Qt::DisplayRole + 1,
    };
    explicit CaptureImageModel();

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetImageCount(int count);
    void setImage(int index, std::string source);

  private:
    std::vector<std::string> image_sources_;
};

class CaptureController : public QObject
{
    Q_OBJECT

  public:
    explicit CaptureController(std::shared_ptr<ICamera> camera);

  Q_SIGNALS:
    void imageCaptured(int id);

  private:
    std::shared_ptr<ICamera> camera_;
    CaptureImageModel capture_model_;
    int current_image_count_{0};
    int image_count_collage_{3};
};
} // namespace Pbox