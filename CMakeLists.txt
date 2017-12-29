cmake_minimum_required( VERSION 2.8 )
project(VulkanTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${VulkanTest_SOURCE_DIR}" )

find_package(PkgConfig)
pkg_check_modules(VULKAN REQUIRED vulkan)
pkg_check_modules(GLFW3 REQUIRED glfw3)

set(SOURCES shaders.cpp VulkanInitialization.cpp VulkanClass.cpp VulkanClass.h main.cpp)
add_executable(VulkanTest ${SOURCES})

include_directories(${VULKAN_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

target_link_libraries(VulkanTest ${VULKAN_LDFLAGS} ${GLFW3_STATIC_LIBRARIES})
