cmake_minimum_required(VERSION 3.0.0)
project(VulkanExample VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(CTest)
enable_testing()

#find_package(glfw3 3.3 REQUIRED)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_executable(VulkanTest ${SRC_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

#target_compile_options(VulkanTest PUBLIC cxx_std_17)
target_link_libraries(VulkanTest PUBLIC glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)
include(CPack)

add_custom_command(TARGET VulkanTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:VulkanTest>/shaders)