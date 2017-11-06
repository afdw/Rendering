cmake_minimum_required(VERSION 2.8)
project(Rendering C)
option(NO_RENDERING "Disable rendering" OFF)
add_executable(${PROJECT_NAME}
    "main.c"
    "Utils.h"
    "VectorTypes.h"
    "Vector2I.h" "Vector2I.c"
    "Vector3F.h" "Vector3F.c"
    "Vector4F.h" "Vector4F.c"
    "Matrix4F.h" "Matrix4F.c"
    "Color.h" "Color.c"
    "Rasterizer.c" "Rasterizer.h"
    "System.h" "System.c"
)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
target_link_libraries(${PROJECT_NAME} m)
if(NOT NO_RENDERING)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
    # find_package(PkgConfig REQUIRED)
    # pkg_search_module(GLFW REQUIRED glfw3)
    # include_directories(${GLFW_INCLUDE_DIRS})
    # target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
    set(GLFW_INSTALL OFF CACHE BOOL "")
    add_subdirectory(glfw)
    target_link_libraries(${PROJECT_NAME} glfw)
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        target_link_libraries(${PROJECT_NAME} libpthread.a)
    elseif()
        set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
        set(THREADS_PREFER_PTHREAD_FLAG TRUE)
        find_package(Threads REQUIRED)
        if(NOT CMAKE_USE_PTHREADS_INIT)
          message(FATAL_ERROR "pthreads library is needed")
        endif()
    endif()
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()
if(NO_RENDERING)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NO_RENDERING)
endif()
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    if(NOT NO_RENDERING)
        target_compile_options(glfw PRIVATE "-Wformat=0")
    endif()
    set(COMMON_COMPILE_FLAGS "-Wall -Wextra -pedantic")
endif()
