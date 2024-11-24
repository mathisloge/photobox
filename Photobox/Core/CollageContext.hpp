#pragma once
#include <Pbox/DisableCopyMove.hpp>
#include <exec/async_scope.hpp>
#include "CollageFontCache.hpp"
#include "CollageRenderer.hpp"
#include "CollageSettings.hpp"
#include "Scheduler.hpp"
#include "SystemStatusClient.hpp"

namespace Pbox
{
class Scheduler;
class CollagePrinter;
class CollageContext final
{
  public:
    PBOX_DISABLE_COPY_MOVE(CollageContext);
    CollageContext(Scheduler &scheduler,
                   std::filesystem::path collage_directory,
                   std::optional<std::filesystem::path> printer_settings = std::nullopt);
    ~CollageContext();

    Scheduler &scheduler();
    CollageRenderer &renderer();
    const CollageSettings &settings() const;
    const SystemStatusClient &systemStatusClient();
    stdexec::sender auto asyncSaveAndPrintCollage()
    {
        return stdexec::then(stdexec::schedule(scheduler_.getSvgRenderScheduler()),
                             [this]() { saveAndPrintCollage(); });
    }

  private:
    void updateSystemStatus();
    void saveAndPrintCollage();

  private:
    SystemStatusClient system_status_client_;
    Scheduler &scheduler_;
    exec::async_scope async_scope_;
    CollageFontCache font_cache_;
    CollageSettings settings_;
    CollageRenderer renderer_;
    std::unique_ptr<CollagePrinter> printer_;
    bool printer_required_{false};
    bool document_ready_{false};
};
} // namespace Pbox
