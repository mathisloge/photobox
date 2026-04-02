#pragma once
#include "CaptureSessionFactory.hpp"
#include "TriggerId.hpp"
namespace Pbox
{
class CaptureSessionManager
{
  public:
    void registerCaptureSession(CaptureSessionId session_id, std::unique_ptr<ICaptureSessionFactory> session_factory);
    void addTriggerRelation(CaptureSessionId session_id, TriggerId trigger_id);
    void setInitialCountdown(std::chrono::seconds initial_countdown);
    CaptureSessionPtr createIdleSession() const;
    CaptureSessionPtr createFromTrigger(const TriggerId &trigger_id) const;

  private:
    std::unordered_map<CaptureSessionId, std::unique_ptr<ICaptureSessionFactory>> session_factories_;
    std::unordered_map<TriggerId, CaptureSessionId> trigger_session_relation_;
    std::chrono::seconds initial_countdown_{};
};
} // namespace Pbox
