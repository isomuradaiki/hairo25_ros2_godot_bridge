#pragma once
#ifndef _ROS2_BRIDGE
#define _ROS2_BRIDGE
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace godot;

#include <vector>
#include <memory>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "ros2_publisher.h"
#include "ros2_subscriber.h"

class ROS2Publisher;  // ←前方宣言
class ROS2Subscriber; // ←同様に前方宣言

class ROS2Bridge : public Object {
    GDCLASS(ROS2Bridge, Object);

    

    std::shared_ptr<rclcpp::Node> node;
    std::vector<std::shared_ptr<ROS2Publisher>> publishers;
    std::vector<std::shared_ptr<ROS2Subscriber>> subscribers;
    std::atomic<bool> alive{false};//追加


protected:
    static void _bind_methods();

public:
    void create_node(const String &node_name);

    void spin_some();

    void shutdown();

    void add_publisher(const String &topic, int type);

    void publish_string(const String &topic, const String &msg);

    void publish_int32(const String &topic,const int32_t &msg);

    void publish_int16_array(const String &topic,const PackedInt32Array &msg);

    void publish_float32_array(const String &topic,const PackedFloat32Array &msg);

    void add_subscriber_string(const String &topic, Callable callback);

    void add_subscriber_int32(const String &topic, Callable callback);

    void add_subscriber_image(const String &topic, Callable callback);

    void add_subscriber_compressed_image(const String &topic, Callable callback);

    void add_subscriber_int16_array(const String &topic, Callable callback);

    void add_subscriber_float32_array(const String &topic, Callable callback);

    //追加
    void remove_publisher(const String &topic);
    void remove_subscriber(const String &topic);

    enum MSG_TYPE {
        STRING,
        INT32,
        IMAGE,
        INT16_ARRAY,
        FLOAT32_ARRAY,
        COMPRESSED_IMAGE,
    };
    

};

VARIANT_ENUM_CAST(ROS2Bridge::MSG_TYPE);

#endif