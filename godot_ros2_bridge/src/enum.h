#pragma once
#include <godot_cpp/core/class_db.hpp>
// グローバル enum (Godotバインド用)
/*    enum ROS_MSG_TYPE {
        ROS2_STRING,
        ROS2_INT32
    };
*/
enum ROS_MSG_TYPE {
        ROS2_STRING,
        ROS2_INT32,
        ROS2_IMAGE,
        ROS2_INT16_ARRAY,
        ROS2_FLOAT32_ARRAY
};
#include "ros2_bridge.h"
