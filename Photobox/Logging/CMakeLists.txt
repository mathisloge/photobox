add_library(PhotoboxLogging)
add_library(Photobox::Logging ALIAS PhotoboxLogging)
set_target_properties(PhotoboxLogging PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
generate_export_header(PhotoboxLogging EXPORT_FILE_NAME LoggingExport.hpp)

target_include_directories(PhotoboxLogging PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(PhotoboxLogging PUBLIC
    quill::quill
)

target_sources(PhotoboxLogging
    PRIVATE
        src/Logger.cpp
)
