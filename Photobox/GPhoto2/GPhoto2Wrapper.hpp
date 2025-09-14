// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <memory>
#include <gphoto2-abilities-list.h>
#include <gphoto2-camera.h>
#include <gphoto2-context.h>
#include <gphoto2-list.h>

namespace Pbox::GPhoto2
{
using ContextUniquePtr = std::unique_ptr<GPContext, decltype(&gp_context_unref)>;
ContextUniquePtr makeUniqueContext();

struct CameraDeleter
{
    GPContext *context;
    void operator()(Camera *c) const;
};
using CameraUniquePtr = std::unique_ptr<Camera, CameraDeleter>;
CameraUniquePtr makeUniqueCamera(GPContext *context);

using CameraListUniquePtr = std::unique_ptr<CameraList, decltype(&gp_list_free)>;
CameraListUniquePtr makeUniqueCameraList();

using CameraAbilitiesListUniquePtr = std::unique_ptr<CameraAbilitiesList, decltype(&gp_abilities_list_free)>;
CameraAbilitiesListUniquePtr makeUniqueCameraListAbilities();

using PortInfoListUniquePtr = std::unique_ptr<GPPortInfoList, decltype(&gp_port_info_list_free)>;
PortInfoListUniquePtr makeUniquePortInfoList();

using CameraFileUniquePtr = std::unique_ptr<CameraFile, decltype(&gp_file_free)>;
CameraFileUniquePtr makeUniqueCameraFile();
} // namespace Pbox::GPhoto2
