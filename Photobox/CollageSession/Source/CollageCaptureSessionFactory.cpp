// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Sessions/CollageCaptureSessionFactory.hpp"
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include "CollageCaptureSession.hpp"
#include "Pbox/Sessions/CollageRenderer.hpp"

DEFINE_LOGGER(collage_capture_session);

namespace Pbox
{
CollageCaptureSessionFactory::CollageCaptureSessionFactory(std::string name,
                                                           Instance<Scheduler> scheduler,
                                                           Instance<ImageStorage> image_storage,
                                                           CollageSettings collage_settings)
    : name_{std::move(name)}
    , scheduler_{std::move(scheduler)}
    , image_storage_{std::move(image_storage)}
    , collage_settings_{std::move(collage_settings)}
    , renderer_{std::make_shared<CollageRenderer>()}
{
    LOG_DEBUG(logger_collage_capture_session(),
              "Create CollageCaptureSessionFactory with svg template: {}",
              collage_settings_.svg_template.string());
    loadCollage();
}

CollageCaptureSessionFactory::~CollageCaptureSessionFactory()
{
    async_scope_.request_stop();
    cleanup_async_scope(async_scope_);
}

void CollageCaptureSessionFactory::loadCollage()
{
    auto init_collage = stdexec::schedule(scheduler_->getWorkScheduler()) | //
                        stdexec::then([this]() {
                            renderer_->loadDocument(collage_settings_.svg_template);
                            for (auto &&element : collage_settings_.image_elements)
                            {
                                renderer_->addPhotoElement(element);
                            }
                        });
    async_scope_.spawn(std::move(init_collage));
}

CaptureSessionPtr CollageCaptureSessionFactory::create(std::chrono::seconds initial_countdown) const
{
    return make_unique_object_ptr_as<ICaptureSession, CollageCaptureSession>(
        name_, initial_countdown, image_storage_, renderer_, scheduler_, collage_settings_);
}
} // namespace Pbox
