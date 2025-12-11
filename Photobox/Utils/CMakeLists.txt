# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxUtils INTERFACE)
add_library(Photobox::Utils ALIAS PhotoboxUtils)
target_link_libraries(
    PhotoboxUtils
    INTERFACE Boost::headers STDEXEC::stdexec Qt6::Core
)
target_sources(
    PhotoboxUtils
    INTERFACE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS include
            FILES
                include/Pbox/CleanupAsyncScope.hpp
                include/Pbox/Conditional.hpp
                include/Pbox/DisableCopyMove.hpp
                include/Pbox/Instance.hpp
                include/Pbox/ObjectDeleter.hpp
                include/Pbox/ObjectUniquePtr.hpp
                include/Pbox/PrintLicenseInfo.hpp
                include/Pbox/QStdexec.hpp
)
