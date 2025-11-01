#pragma once
#ifndef _ROS2_PUBLISHER_H
#define _ROS2_PUBLISHER_H

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/int16_multi_array.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <memory>
#include <string>
#include "enum.h"
class ROS2Publisher {
public:
    std::string topic;
    ROS_MSG_TYPE type;

    // 型ごとのPublisher
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_string;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_int;
    rclcpp::Publisher<std_msgs::msg::Int16MultiArray>::SharedPtr pub_int16_array;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_float32_array;

    ROS2Publisher(std::shared_ptr<rclcpp::Node> node,
                  const std::string &topic_name,
                  ROS_MSG_TYPE msg_type);
    void publish_string(const std::string &msg);

    void publish_int(const int value);

    void publish_int16_array(const PackedInt32Array &array);

    void publish_float32_array(const PackedFloat32Array &array);
};
#endif