configure_file(build_and_publish_docker.sh build_and_publish_docker.sh USE_SOURCE_PERMISSIONS @ONLY)
add_custom_target(BuildDockerImageForCI "${CMAKE_CURRENT_BINARY_DIR}/build_and_publish_docker.sh"
    DEPENDS Dockerfile
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
