#pragma once
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
    CollageCaptureSessionFactory(Instance<Scheduler> scheduler,
                                 Instance<CollageRenderer> renderer,
                                 Instance<CollagePrinter> printer,
                                 Instance<ImageStorage> image_storage,
                                 CollageSettings collage_settings);
    CaptureSessionPtr create() const override;

  private:
    Instance<Scheduler> scheduler_;
    Instance<CollageRenderer> renderer_;
    Instance<CollagePrinter> printer_;
    Instance<ImageStorage> image_storage_;
    CollageSettings collage_settings_;
};
} // namespace Pbox
