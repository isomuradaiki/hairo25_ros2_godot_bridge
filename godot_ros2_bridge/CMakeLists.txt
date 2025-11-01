cmake_minimum_required(VERSION 3.15)
project(godot_ros2_bridge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# Godot-cpp
set(GODOT_CPP_PATH "${CMAKE_SOURCE_DIR}/../godot-cpp")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GODOT_CPP_LIB "${GODOT_CPP_PATH}/bin/libgodot-cpp.linux.template_debug.x86_64.a")
else()
    set(GODOT_CPP_LIB "${GODOT_CPP_PATH}/bin/libgodot-cpp.linux.template_release.x86_64.a")
endif()

include_directories(
    ${GODOT_CPP_PATH}/include
    ${GODOT_CPP_PATH}/include/godot_cpp
    ${GODOT_CPP_PATH}/gen/include
    ${GODOT_CPP_PATH}/gdextension                 # ← これを追加！
)

#file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB SOURCE_FILES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

ament_target_dependencies(${PROJECT_NAME}
    rclcpp
    std_msgs
    sensor_msgs
)

target_link_libraries(${PROJECT_NAME}
    ${GODOT_CPP_LIB}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    BUILD_RPATH "/opt/ros/jazzy/lib"
    INSTALL_RPATH "/opt/ros/jazzy/lib"
)

ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)
