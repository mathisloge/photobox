add_library(PhotoboxUtils INTERFACE)
add_library(Photobox::Utils ALIAS PhotoboxUtils)
target_link_libraries(PhotoboxUtils INTERFACE
    STDEXEC::stdexec
    Qt6::Core
)

target_include_directories(PhotoboxUtils INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
