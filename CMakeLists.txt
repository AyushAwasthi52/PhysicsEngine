cmake_minimum_required(VERSION 3.25)
project(PhysicsEngine2D LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(SDL3  REQUIRED CONFIG)
find_package(glm   REQUIRED CONFIG)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(PhysicsEngine2D ${SOURCES})

target_include_directories(PhysicsEngine2D PRIVATE src)
target_link_libraries(PhysicsEngine2D PRIVATE
    Vulkan::Vulkan
    SDL3::SDL3
    glm::glm
)

# Shader compilation
find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/bin)
file(GLOB SHADERS shaders/*.vert shaders/*.frag)
foreach(SHADER ${SHADERS})
    get_filename_component(NAME ${SHADER} NAME)
    add_custom_command(TARGET PhysicsEngine2D POST_BUILD
        COMMAND ${GLSLC} ${SHADER} -o
                $<TARGET_FILE_DIR:PhysicsEngine2D>/shaders/${NAME}.spv)
endforeach()