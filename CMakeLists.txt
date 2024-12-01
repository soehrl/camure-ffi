cmake_minimum_required(VERSION 3.11)

include(ExternalProject)

project(
    camure
    VERSION 0.1.0
    DESCRIPTION "High-performance 1-to-many communication and synchronization primitives using UDP multicast."
    LANGUAGES C
)

find_program(
    CARGO
    NAMES cargo
    DOC "Path to cargo"
    REQUIRED
)
message(STATUS "Found cargo at ${CARGO}")
ExternalProject_Add(
    camure_ffi
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND} -E env CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR}/camure_ffi cargo build --release
    UPDATE_COMMAND ${CMAKE_COMMAND} -E env CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR}/camure_ffi cargo build --release
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/camure_ffi"
    INSTALL_COMMAND ""
    LOG_BUILD ON
)

add_library(
    camure
    STATIC
    IMPORTED
)
set_target_properties(
    camure
    PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/camure_ffi/release/${CMAKE_STATIC_LIBRARY_PREFIX}camure_ffi${CMAKE_STATIC_LIBRARY_SUFFIX}
)
target_include_directories(
    camure
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (PROJECT_IS_TOP_LEVEL)
    add_executable(
        barrier_group_coordinator
        examples/barrier_group_coordinator.c
    )
    target_link_libraries(
        barrier_group_coordinator
        PRIVATE
            camure
    )

    add_executable(
        barrier_group_member
        examples/barrier_group_member.c
    )
    target_link_libraries(
       barrier_group_member 
        PRIVATE
            camure
    )

    add_executable(
        broadcast_group_sender
        examples/broadcast_group_sender.c
    )
    target_link_libraries(
        broadcast_group_sender 
        PRIVATE
            camure
    )

    add_executable(
        broadcast_group_receiver
        examples/broadcast_group_receiver.c
    )
    target_link_libraries(
        broadcast_group_receiver 
        PRIVATE
            camure
    )
endif ()
