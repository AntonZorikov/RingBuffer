cmake_minimum_required(VERSION 3.5)

project(RingBuffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(googletest-release-1.12.1)
add_executable(RingBuffer main.cpp test.cpp headers/ringbuffer.h headers/ringbuffer_client.h)
target_link_libraries(RingBuffer gtest_main gmock_main)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(RingBuffer Threads::Threads)

include_directories("headers/")

if(CMAKE_SYSTEM_NAME STREQUAL "KPDA")
    set(INSTALL_DESTDIR "/tmp")

    install(TARGETS RingBuffer
        RUNTIME DESTINATION "${INSTALL_DESTDIR}"
        BUNDLE DESTINATION "${INSTALL_DESTDIR}"
        LIBRARY DESTINATION "${INSTALL_DESTDIR}"
    )
endif()
