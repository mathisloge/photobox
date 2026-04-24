#pragma once
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QColor>
#include <CaptureSessionManager.hpp>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox::Ui
{
/**
 * @brief Model for listing capture sessions in the UI.
 *
 * This model provides a list of capture sessions with their IDs, names, and colors.
 */
class CaptureSessionList : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Pbox::CaptureSessionManager *sessionManager MEMBER session_manager_ NOTIFY sessionManagerChanged)
  public:
    /**
     * @brief Roles for the model data.
     */
    enum class Role
    {
        SessionId = Qt::DisplayRole + 1, // The ID of the capture session.
        Name,                            // The name of the capture session.
        Color,                           // The color associated with the session.
    };

  public:
    /**
     * @brief Constructs a CaptureSessionList.
     */
    CaptureSessionList();
    /**
     * @brief Returns the number of rows in the model.
     * @param parent The parent model index.
     * @return The number of sessions.
     */
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    /**
     * @brief Returns the data for the given index and role.
     * @param index The model index.
     * @param role The role to query.
     * @return The data for the role.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    /**
     * @brief Returns the role names for QML.
     * @return A hash of role names.
     */
    QHash<int, QByteArray> roleNames() const override;

  Q_SIGNALS:
    /**
     * @brief Emitted when the session manager changes.
     */
    void sessionManagerChanged();

  private:
    /**
     * @brief Handles changes in the session manager.
     */
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
