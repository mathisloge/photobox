#pragma once
#include <QAbstractListModel>
#include <QtQmlIntegration>
#include <memory>
#include <Pbox/DisableCopyMove.hpp>
#include "SystemStatusCode.hpp"

namespace Pbox
{
template <typename T>
concept SystemStatusClientConcept = requires(T c) {
    { c.systemStatus() } -> std::convertible_to<SystemStatusCode::Code>;
};

class SystemStatusClient
{
    class ClientConcept
    {
      public:
        PBOX_DISABLE_COPY_MOVE(ClientConcept);
        virtual ~ClientConcept() = default;
        virtual SystemStatusCode::Code systemStatus() const = 0;
    };

    template <SystemStatusClientConcept T>
    class ClientModel : public ClientConcept
    {
        const T *client_;

      public:
        ClientModel(const T *client)
            : client_(client)
        {}

        SystemStatusCode::Code systemStatus() const override
        {
            return client_->systemStatus();
        }
    };

  public:
    PBOX_DISABLE_COPY(SystemStatusClient);

    template <SystemStatusClientConcept T>
    SystemStatusClient(T *client, QString name)
        : client_{std::make_unique<ClientModel<T>>(std::forward<T>(client))}
        , name_{std::move(name)}
    {}

    ~SystemStatusClient() = default;

    SystemStatusClient(SystemStatusClient &&rhs) noexcept
        : name_{std::move(rhs.name_)}
        , client_{std::move(rhs.client_)}
    {}

    SystemStatusClient &operator=(SystemStatusClient &&rhs) noexcept
    {
        name_ = std::move(rhs.name_);
        client_ = std::move(rhs.client_);
        return *this;
    }

    SystemStatusCode::Code systemStatus() const
    {
        return client_->systemStatus();
    }

    const QString &name() const
    {
        return name_;
    }

  private:
    QString name_;
    std::unique_ptr<ClientConcept> client_;
};

class SystemStatusModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Will be provided by C++")
  public:
    enum class Role
    {
        Title = Qt::DisplayRole + 1,
        Status,
    };

  public:
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void registerClient(SystemStatusClient &&client);

  private:
    std::vector<SystemStatusClient> clients_;
};
} // namespace Pbox
