// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Sessions/CollageCaptureSessionFactory.hpp"
#include "CollageCaptureSession.hpp"
#include "Pbox/CleanupAsyncScope.hpp"
#include "Pbox/Sessions/CollageRenderer.hpp"

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

CaptureSessionPtr CollageCaptureSessionFactory::create() const
{
    return make_unique_object_ptr_as<ICaptureSession, CollageCaptureSession>(
        name_, image_storage_, renderer_, scheduler_, collage_settings_);
}
} // namespace Pbox
