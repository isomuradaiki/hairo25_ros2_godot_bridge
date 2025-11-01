#pragma once
#ifndef _ROS_SUBSCRIPTION_H
#define _ROS_SUBSCRIPTION_H

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/int16_multi_array.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <functional>
#include <memory>
#include "enum.h"
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class ROS2Subscriber {
public:
    std::string topic;
    ROS_MSG_TYPE type;

    // 型ごとのSubscriber
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_string;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_int;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_image;
    rclcpp::Subscription<std_msgs::msg::Int16MultiArray>::SharedPtr sub_int16_array;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_float32_array;

    // Godotコールバック用
    std::function<void(const std::string&)> callback_string;
    std::function<void(int)> callback_int;
    Callable callback_image;
    std::function<void(PackedInt32Array)> callback_int16_array;
    std::function<void(PackedFloat32Array)> callback_float32_array;
    //ここに書きたい

    ROS2Subscriber(std::shared_ptr<rclcpp::Node> node,
                   const std::string &topic_name,
                   ROS_MSG_TYPE msg_type,
                   std::function<void(const std::string&)> cb_str = nullptr,
                   std::function<void(int)> cb_int = nullptr,
                   Callable cb_image = Callable(),
                   std::function<void(PackedInt32Array)> cb_int16_array = nullptr,
                   std::function<void(PackedFloat32Array)> cb_float32_array = nullptr
                );
    private:
    void setup_image_subscription(std::shared_ptr<rclcpp::Node> node);
};
#endif