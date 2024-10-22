# see https://github.com/jeremy-rifkin/cpptrace/blob/main/docs/signal-safe-tracing.md
add_executable(PhotoboxSignalTracer main.cpp)
target_link_libraries(PhotoboxSignalTracer PRIVATE cpptrace::cpptrace)

install(TARGETS PhotoboxSignalTracer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
