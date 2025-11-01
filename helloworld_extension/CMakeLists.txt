cmake_minimum_required(VERSION 3.15)
project(helloworld_extension)

set(CMAKE_CXX_STANDARD 17)

# Godot-cppのパスを設定
set(GODOT_CPP_PATH "${CMAKE_SOURCE_DIR}/../godot-cpp")

# Debug/Releaseごとにリンクする静的ライブラリを分岐
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GODOT_CPP_LIB "${GODOT_CPP_PATH}/bin/libgodot-cpp.linux.template_debug.x86_64.a")
else()
    set(GODOT_CPP_LIB "${GODOT_CPP_PATH}//bin/libgodot-cpp.linux.template_release.x86_64.a")
endif()

include_directories(
    ${GODOT_CPP_PATH}/include
    ${GODOT_CPP_PATH}/include/godot_cpp
    ${GODOT_CPP_PATH}/gen/include
    ${GODOT_CPP_PATH}/src
    ${GODOT_CPP_PATH}//gdextension                 # ← これを追加！
)

add_library(${PROJECT_NAME} SHARED
    src/godot_ros2_publisher.cpp
)

# 追加
target_link_libraries(${PROJECT_NAME} ${GODOT_CPP_LIB})

#既存
#target_link_libraries(${PROJECT_NAME}
#    ${GODOT_CPP_PATH}/bin/libgodot-cpp.linux.template_debug.x86_64.a
#)


