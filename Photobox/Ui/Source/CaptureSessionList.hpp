#pragma once
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QColor>
#include <CaptureSessionManager.hpp>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox::Ui
{
class CaptureSessionList : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Pbox::CaptureSessionManager *sessionManager MEMBER session_manager_ NOTIFY sessionManagerChanged)
  public:
    enum class Role
    {
        SessionId = Qt::DisplayRole + 1,
        Name,
        Color,
    };

  public:
    CaptureSessionList();
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

  Q_SIGNALS:
    void sessionManagerChanged();

  private:
    void onSessionManagerChanged();

  private:
    CaptureSessionManager *session_manager_;

    struct SessionData
    {
        QString sessionId;
        QString name;
        QColor color;
    };
    std::vector<SessionData> sessions_;
};
} // namespace Pbox::Ui
