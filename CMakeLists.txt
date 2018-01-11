cmake_minimum_required(VERSION 3.0)
project(Simulation)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Simulation/Vendor/glfw)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Simulation/Headers/
                    Simulation/Vendor/glad/include/
                    Simulation/Vendor/glfw/include/
                    Simulation/Vendor/glm/
                    Simulation/Vendor/stb/)

file(GLOB VENDORS_SOURCES Simulation/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Simulation/Headers/*.hpp
                          Simulation/Headers/*.h)
file(GLOB PROJECT_SOURCES Simulation/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Simulation/Shaders/*.comp
                          Simulation/Shaders/*.frag
                          Simulation/Shaders/*.geom
                          Simulation/Shaders/*.vert)
file(GLOB PROJECT_RESOURCES Simulation/Resources/*.png
                            Simulation/Resources/*.jpg)                          
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} ${PROJECT_RESOURCES})
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      )
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_target(shader COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/Simulation/Shaders/
                   ${CMAKE_BINARY_DIR}/Simulation/Shaders/)

add_dependencies(${PROJECT_NAME} shader)

add_custom_target(resource COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/Simulation/Resources/
                   ${CMAKE_BINARY_DIR}/Simulation/Resources/)

add_dependencies(${PROJECT_NAME} resource)
