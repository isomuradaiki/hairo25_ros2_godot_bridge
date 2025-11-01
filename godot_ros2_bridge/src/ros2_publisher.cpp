#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/int16_multi_array.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <memory>
#include <string>
#include "enum.h"
#include "ros2_publisher.h"

ROS2Publisher::ROS2Publisher(std::shared_ptr<rclcpp::Node> node,
                const std::string &topic_name,
                ROS_MSG_TYPE msg_type)
    : topic(topic_name), type(msg_type) {

    if (type == ROS2_STRING) {
        pub_string = node->create_publisher<std_msgs::msg::String>(topic, 10);
    } else if (type == ROS2_INT32) {
        pub_int = node->create_publisher<std_msgs::msg::Int32>(topic, 10);
    } else if (type == ROS2_INT16_ARRAY){
        pub_int16_array = node->create_publisher<std_msgs::msg::Int16MultiArray>(topic,10);
    } else if (type == ROS2_FLOAT32_ARRAY){
        pub_float32_array = node->create_publisher<std_msgs::msg::Float32MultiArray>(topic,10);
    }
}

void ROS2Publisher::publish_string(const std::string &msg) {
    if (pub_string) {
        std_msgs::msg::String m;
        m.data = msg;
        pub_string->publish(m);
    }
}

void ROS2Publisher::publish_int(const int value) {
    if (pub_int) {
        std_msgs::msg::Int32 m;
        m.data = value;
        pub_int->publish(m);
    }
}

void ROS2Publisher::publish_int16_array(const PackedInt32Array &array){
    std_msgs::msg::Int16MultiArray ros_msg;
    ros_msg.data.resize(array.size());
    for (int i=0;i<array.size();i++){
        ros_msg.data[i]=static_cast<int16_t>(array[i]);
    }
    pub_int16_array->publish(ros_msg);
}

void ROS2Publisher::publish_float32_array(const PackedFloat32Array &array){
    std_msgs::msg::Float32MultiArray ros_msg;
    ros_msg.data.resize(array.size());
    for (int i=0;i<array.size();i++){
        ros_msg.data[i]=static_cast<_Float16>(array[i]);
    }
    pub_float32_array->publish(ros_msg);
}