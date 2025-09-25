// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <exec/async_scope.hpp>
#include "CaptureSessionFactory.hpp"
#include "CollageSettings.hpp"
#include "ImageStorage.hpp"
#include "Pbox/Instance.hpp"
#include "Scheduler.hpp"

namespace Pbox
{
class CollageRenderer;
class CollageCaptureSessionFactory : public ICaptureSessionFactory
{
  public:
    PBOX_DISABLE_COPY_MOVE(CollageCaptureSessionFactory)
    CollageCaptureSessionFactory(std::string name,
                                 Instance<Scheduler> scheduler,
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
    Instance<ImageStorage> image_storage_;
    CollageSettings collage_settings_;
    exec::async_scope async_scope_;
};
} // namespace Pbox
