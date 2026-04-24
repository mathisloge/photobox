#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include "CaptureSessionFactory.hpp"
#include "TriggerId.hpp"
namespace Pbox
{
/**
 * @brief Manager for registering and creating capture sessions.
 *
 * This class manages the registration of capture session factories and
 * the relationships between triggers and sessions. It provides methods
 * to create sessions based on triggers or session IDs.
 */
class CaptureSessionManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")
  public:
    /**
     * @brief Registers a capture session factory.
     * @param session_id The unique ID for the session.
     * @param session_factory The factory for creating the session.
     */
    void registerCaptureSession(CaptureSessionId session_id, std::unique_ptr<ICaptureSessionFactory> session_factory);
    /**
     * @brief Adds a relationship between a trigger and a session.
     * @param session_id The session ID.
     * @param trigger_id The trigger ID.
     */
    void addTriggerRelation(CaptureSessionId session_id, TriggerId trigger_id);
    /**
     * @brief Sets the initial countdown duration for new sessions.
     * @param initial_countdown The countdown duration.
     */
    void setInitialCountdown(std::chrono::seconds initial_countdown);
    /**
     * @brief Creates an idle session.
     * @return A unique pointer to the idle session.
     */
    CaptureSessionPtr createIdleSession() const;
    /**
     * @brief Creates a session based on a trigger ID.
     * @param trigger_id The trigger ID.
     * @return A unique pointer to the session.
     */
    CaptureSessionPtr createFromTrigger(const TriggerId &trigger_id) const;
    /**
     * @brief Creates a session based on a session ID.
     * @param session_id The session ID.
     * @return A unique pointer to the session.
     */
    CaptureSessionPtr createFromSessionId(const CaptureSessionId &session_id) const;

    /**
     * @brief Gets all registered session factories.
     * @return A range of session factories.
     */
    auto getSessionFactories() const
    {
        return session_factories_ | std::views::values |
               std::views::transform([](const auto &ptr) -> const ICaptureSessionFactory & { return *ptr; });
    }

  private:
    std::unordered_map<CaptureSessionId, std::unique_ptr<ICaptureSessionFactory>> session_factories_;
    std::unordered_map<TriggerId, CaptureSessionId> trigger_session_relation_;
    std::chrono::seconds initial_countdown_{};
};
} // namespace Pbox
