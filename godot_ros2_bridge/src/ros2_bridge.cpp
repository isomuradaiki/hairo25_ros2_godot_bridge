#include "ros2_bridge.h"

void ROS2Bridge::_bind_methods() {
    //node
    ClassDB::bind_method(D_METHOD("create_node","node_name"), &ROS2Bridge::create_node);
    ClassDB::bind_method(D_METHOD("spin_some"), &ROS2Bridge::spin_some);
    ClassDB::bind_method(D_METHOD("shutdown"), &ROS2Bridge::shutdown);
    //publish
    ClassDB::bind_method(D_METHOD("add_publisher", "topic", "type"), &ROS2Bridge::add_publisher);
    ClassDB::bind_method(D_METHOD("publish_string", "topic", "msg"), &ROS2Bridge::publish_string);
    ClassDB::bind_method(D_METHOD("publish_int32","topic","msg"),&ROS2Bridge::publish_int32);
    ClassDB::bind_method(D_METHOD("publish_int16_array","topic","msg"),&ROS2Bridge::publish_int16_array);
    ClassDB::bind_method(D_METHOD("publish_float32_array","topic","msg"),&ROS2Bridge::publish_float32_array);
    //subscribe
    ClassDB::bind_method(D_METHOD("add_subscriber_string", "topic", "callback"), &ROS2Bridge::add_subscriber_string);
    ClassDB::bind_method(D_METHOD("add_subscriber_int32", "topic", "callback"), &ROS2Bridge::add_subscriber_int32);
    ClassDB::bind_method(D_METHOD("add_subscriber_int16_array", "topic", "callback"), &ROS2Bridge::add_subscriber_int16_array);
    ClassDB::bind_method(D_METHOD("add_subscriber_float32_array", "topic", "callback"), &ROS2Bridge::add_subscriber_float32_array);
    ClassDB::bind_method(D_METHOD("add_subscriber_image","topic","callback"),&ROS2Bridge::add_subscriber_image);
    ClassDB::bind_method(D_METHOD("add_subscriber_compressed_image","topic","callback"),&ROS2Bridge::add_subscriber_compressed_image);//圧縮画像
    //topic通信の破壊
    ClassDB::bind_method(D_METHOD("remove_publisher","topic"),&ROS2Bridge::remove_publisher);
    ClassDB::bind_method(D_METHOD("remove_subscriber","topic"),&ROS2Bridge::remove_subscriber);
    // enumバインド
    BIND_ENUM_CONSTANT(STRING);
    BIND_ENUM_CONSTANT(INT32);
    BIND_ENUM_CONSTANT(IMAGE);
    BIND_ENUM_CONSTANT(INT16_ARRAY);
    BIND_ENUM_CONSTANT(FLOAT32_ARRAY);
}
void ROS2Bridge::create_node(const String &node_name) {
    if (alive) return;//追加
    if (!rclcpp::ok()) rclcpp::init(0, nullptr);
    node = rclcpp::Node::make_shared(node_name.utf8().get_data());
    UtilityFunctions::print("ROS2 node started!");
    alive = true;//追加
}

void ROS2Bridge::spin_some() {
    if (node) rclcpp::spin_some(node);
}

void ROS2Bridge::shutdown() {
    if (!alive) return;//追加
    publishers.clear();
    subscribers.clear();
    // spin_some などで使っている node があれば
    if (node) {
        node.reset();  // すべての subscriber/publisher もここで自動的に破棄される
    
    }
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
    UtilityFunctions::print("ROS2 node shutdown!");
}

void ROS2Bridge::add_publisher(const String &topic, int type) {
    publishers.push_back(std::make_shared<ROS2Publisher>(node, topic.utf8().get_data(), static_cast<ROS_MSG_TYPE>(type)));
}

void ROS2Bridge::publish_string(const String &topic, const String &msg) {
    for (auto &pub : publishers) {
        if (pub->topic == topic.utf8().get_data() && pub->type == ROS2_STRING) {
            pub->publish_string(msg.utf8().get_data());
        }
    }
}

void ROS2Bridge::publish_int32(const String &topic,const int32_t &msg) {
    for (auto &pub : publishers){
        if(pub->topic == topic.utf8().get_data() && pub->type == ROS2_INT32){
            pub->publish_int(msg);
        }
    }
}

void ROS2Bridge::publish_int16_array(const String &topic,const PackedInt32Array &msg){
    for(auto &pub : publishers){
        if(pub->topic == topic.utf8().get_data() && pub->type == ROS2_INT16_ARRAY){
            pub->publish_int16_array(msg);
        }
    }
}

void ROS2Bridge::publish_float32_array(const String &topic,const PackedFloat32Array &msg){
    for(auto &pub : publishers){
        if(pub->topic == topic.utf8().get_data() && pub->type == ROS2_FLOAT32_ARRAY){
            pub->publish_float32_array(msg);
        }
    }
}


void ROS2Bridge::add_subscriber_string(const String &topic, Callable callback) {
    auto cb = [callback](const std::string &msg){
        Variant arg(msg.c_str());
        callback.callv(Array::make(arg));
    };
    subscribers.push_back(std::make_shared<ROS2Subscriber>(node, topic.utf8().get_data(), ROS2_STRING, cb, nullptr));
}

void ROS2Bridge::add_subscriber_int32(const String &topic, Callable callback) {
    auto cb = [callback](const int &msg){
        Variant arg(msg);
        callback.callv(Array::make(arg));
    };
    subscribers.push_back(std::make_shared<ROS2Subscriber>(node, topic.utf8().get_data(), ROS2_INT32, nullptr,cb));
}

void ROS2Bridge::add_subscriber_image(const String &topic, Callable callback) {
    subscribers.push_back(std::make_shared<ROS2Subscriber>(
        node,
        topic.utf8().get_data(),
        ROS2_IMAGE,
        nullptr,    // string callback
        nullptr,    // int callback
        callback    // image callback
    ));
}

void ROS2Bridge::add_subscriber_compressed_image(const String &topic,Callable callback) {
    subscribers.push_back(std::make_shared<ROS2Subscriber>(
        node,
        topic.utf8().get_data(),
        ROS2_COMPRESSED_IMAGE,
        nullptr,    // string callback
        nullptr,    // int callback
        Callable(),
        nullptr,
        nullptr,
        callback
    ));
}

void ROS2Bridge::add_subscriber_int16_array(const String &topic, Callable callback) {
    auto cb = [callback](const PackedInt32Array &msg){
        Variant arg(msg);
        callback.callv(Array::make(arg));
    };
    subscribers.push_back(std::make_shared<ROS2Subscriber>(node, topic.utf8().get_data(), ROS2_INT16_ARRAY, nullptr, nullptr,Callable(),cb));
}

void ROS2Bridge::add_subscriber_float32_array(const String &topic, Callable callback) {
    auto cb = [callback](const PackedFloat32Array &msg){
        Variant arg(msg);
        callback.callv(Array::make(arg));
    };
    subscribers.push_back(std::make_shared<ROS2Subscriber>(node, topic.utf8().get_data(), ROS2_FLOAT32_ARRAY, nullptr, nullptr,Callable(),nullptr,cb));
}

//以下追加
void ROS2Bridge::remove_publisher(const String &topic) {
    std::string t = topic.utf8().get_data();

    publishers.erase(
        std::remove_if(
            publishers.begin(),
            publishers.end(),
            [&](const std::shared_ptr<ROS2Publisher>& pub){
                return pub->topic == t;
            }
        ),
        publishers.end()
    );
}

void ROS2Bridge::remove_subscriber(const String &topic) {
    std::string t = topic.utf8().get_data();

    subscribers.erase(
        std::remove_if(
            subscribers.begin(),
            subscribers.end(),
            [&](const std::shared_ptr<ROS2Subscriber>& sub){
                return sub->topic == t;
            }
        ),
        subscribers.end()
    );
}
