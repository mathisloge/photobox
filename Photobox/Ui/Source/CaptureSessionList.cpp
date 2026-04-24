#include "CaptureSessionList.hpp"
namespace Pbox::Ui
{
CaptureSessionList::CaptureSessionList()
{
    connect(this, &CaptureSessionList::sessionManagerChanged, this, &CaptureSessionList::onSessionManagerChanged);
}

int CaptureSessionList::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return static_cast<int>(sessions_.size());
}

QVariant CaptureSessionList::data(const QModelIndex &index, int role) const
{
    if (sessions_.size() < index.row())
    {
        return {};
    }
    const auto &session = sessions_.at(index.row());
    switch (Role{role})
    {
    case Role::SessionId:
        return session.sessionId;
    case Role::Name:
        return session.name;
    case Role::Color:
        return session.color;
    }
    return {};
}

QHash<int, QByteArray> CaptureSessionList::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::to_underlying(Role::Color), "color"},
        {std::to_underlying(Role::Name), "name"},
        {std::to_underlying(Role::SessionId), "sessionId"},
    };
    return kRoles;
}

void CaptureSessionList::onSessionManagerChanged()
{
    beginResetModel();
    sessions_.clear();
    if (session_manager_ != nullptr)
    {
        for (const ICaptureSessionFactory &session : session_manager_->getSessionFactories())
        {
            sessions_.emplace_back(SessionData{
                .sessionId = QString::fromStdString(session.sessionId()),
                .name = QString::fromStdString(session.name()),
                .color = session.color(),
            });
        }
    }
    endResetModel();
}
} // namespace Pbox::Ui
