cmake_minimum_required(VERSION 3.0)

project (SpinningCubes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
add_subdirectory(lib/glew)
include_directories(lib/glew/include)

# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

set(SOURCE main.cpp Window.cpp Window.h entities/Entity.cpp entities/Entity.h Math.h entities/Cube.cpp entities/Cube.h Renderer.cpp Renderer.h entities/Container.cpp entities/Container.h entities/Modifier.cpp entities/Modifier.h allocators/TransformAllocator.h allocators/TransformSlabAllocator.cpp allocators/TransformSlabAllocator.h allocators/TransformSystemAllocator.cpp allocators/TransformSystemAllocator.h)
add_executable(SpinningCubes ${SOURCE})

target_include_directories(SpinningCubes PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(SpinningCubes ${OPENGL_LIBRARIES})
target_link_libraries(SpinningCubes libglew_shared)
target_link_libraries(SpinningCubes glfw ${GLFW_LIBRARIES})

install(TARGETS SpinningCubes RUNTIME DESTINATION bin)
