cmake_minimum_required(VERSION 3.0.0)
project(VulkanExample VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(CTest)
#enable_testing()

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

# Find all shaders.
file(GLOB SHADERS
  ${SHADER_SOURCE_DIR}/*.vert
  ${SHADER_SOURCE_DIR}/*.frag
  ${SHADER_SOURCE_DIR}/*.comp
  ${SHADER_SOURCE_DIR}/*.geom
  ${SHADER_SOURCE_DIR}/*.tesc
  ${SHADER_SOURCE_DIR}/*.tese
  ${SHADER_SOURCE_DIR}/*.mesh
  ${SHADER_SOURCE_DIR}/*.task
  ${SHADER_SOURCE_DIR}/*.rgen
  ${SHADER_SOURCE_DIR}/*.rchit
  ${SHADER_SOURCE_DIR}/*.rmiss)


add_custom_command(
  COMMAND
    ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
  OUTPUT ${SHADER_BINARY_DIR}
  COMMENT "Creating ${SHADER_BINARY_DIR}"
)

foreach(source IN LISTS SHADERS)
  get_filename_component(FILENAME ${source} NAME)
  add_custom_command(
    COMMAND
      /usr/local/bin/glslc
            -MD -MF ${SHADER_BINARY_DIR}/${FILENAME}.d
      -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
      ${source}
    OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
    DEPENDS ${source} ${SHADER_BINARY_DIR}
    COMMENT "Compiling ${FILENAME}"
  )
  list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach()

add_custom_target(SPV_SHADER ALL DEPENDS ${SPV_SHADERS})

add_executable(VulkanTest ${SRC_FILES} ${SPV_SHADER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

#target_compile_options(VulkanTest PUBLIC cxx_std_17)
target_link_libraries(VulkanTest PUBLIC glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)
include(CPack)
