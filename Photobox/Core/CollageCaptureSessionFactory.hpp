#pragma once
#include <exec/async_scope.hpp>
#include "CaptureSessionFactory.hpp"
#include "CollagePrinter.hpp"
#include "CollageRenderer.hpp"
#include "CollageSettings.hpp"
#include "ImageStorage.hpp"
#include "Pbox/Instance.hpp"
#include "Scheduler.hpp"

namespace Pbox
{
class CollageCaptureSessionFactory : public ICaptureSessionFactory
{
  public:
    PBOX_DISABLE_COPY_MOVE(CollageCaptureSessionFactory)
    CollageCaptureSessionFactory(std::string name,
                                 Instance<Scheduler> scheduler,
                                 Instance<CollagePrinter> printer,
                                 Instance<ImageStorage> image_storage,
                                 CollageSettings collage_settings);
    ~CollageCaptureSessionFactory() override;
    CaptureSessionPtr create() const override;

  private:
    void loadCollage();

  private:
    std::string name_;
    Instance<CollageRenderer> renderer_;
    Instance<Scheduler> scheduler_;
    Instance<CollagePrinter> printer_;
    Instance<ImageStorage> image_storage_;
    CollageSettings collage_settings_;
    exec::async_scope async_scope_;
};
} // namespace Pbox
