#pragma once
#include <QObject>
#include "SystemStatusModel.hpp"

namespace Pbox
{
class SystemStatusManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::SystemStatusModel *model READ getModel CONSTANT FINAL);
    Q_PROPERTY(Pbox::SystemStatusManager::Status status READ getStatus NOTIFY statusChanged FINAL);

  public:
    enum class Status
    {
        Initializing,
        RequiredClientsInitilized,
        AllClientsInitializied
    };
    Q_ENUM(Status);

  public:
    void registerClient(const Pbox::SystemStatusClient *client);

    /// vvv property methods
    SystemStatusModel *getModel();
    Status getStatus() const;
    /// ^^^ property methods
  Q_SIGNALS:
    /// vvv property signals
    void statusChanged();
    /// ^^^ property signals

  private:
    void evaluateSystemStatus();

  private:
    std::vector<QPointer<const SystemStatusClient>> clients_;
    SystemStatusModel model_;
    Status status_{Status::AllClientsInitializied};
};
} // namespace Pbox
