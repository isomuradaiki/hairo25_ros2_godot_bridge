#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_ros2_publisher.h"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace godot;

class ROS2Bridge: public Object{
    GDCLASS(ROS2Bridge,Object);

    std::shared_ptr<rclcpp::Node> node;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    
    protected:
    static void _bind_methods(){
        ClassDB::bind_method(D_METHOD("publish_message","msg"),&ROS2Bridge::publish_message);
        ClassDB::bind_method(D_METHOD("process"),&ROS2Bridge::process);
        ClassDB::bind_method(D_METHOD("ready"),&ROS2Bridge::ready);

    }
    public:
    /*void _ready(){
        if(!rclcpp::ok()){
            rclcpp::init(0,nullptr);
        }
        node = rclcpp::Node::make_shared("godot_ros2_node");
        publisher = node->create_publisher<std_msgs::msg::String>("chatter",10);

        UtilityFunctions::print("ROS2 node started!");
    }*/
    void ready(){
        if(!rclcpp::ok()){
            rclcpp::init(0,nullptr);
        }
        node = rclcpp::Node::make_shared("godot_ros2_node");
        if(node) UtilityFunctions::print("ROS2 node created");
        else UtilityFunctions::print("ROS2 node creation failed");

        publisher = node->create_publisher<std_msgs::msg::String>("chatter",10);
        if(publisher) UtilityFunctions::print("Publisher created");
        else UtilityFunctions::print("Publisher creation failed");

        UtilityFunctions::print("ROS2 node started!");
    }

    void process(){
        rclcpp::spin_some(node);
    }

    void publish_message(const godot::String &msg) {
        if (!publisher) { // Publisherが初期化されていなければ無視
            UtilityFunctions::print("ROS2 not setuped");
            return;
         }

        std_msgs::msg::String ros_msg;
        ros_msg.data = msg.utf8().get_data(); // godot::String → std::string
        publisher->publish(ros_msg);
    }

    ~ROS2Bridge(){
        if(rclcpp::ok())rclcpp::shutdown();
        UtilityFunctions::print("shutdown");
    }
};

//--登録処理--
void initialize_godor_ros2_bridge(ModuleInitializationLevel p_level){
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
    ClassDB::register_class<ROS2Bridge>();
}

void uninitialize_godor_ros2_bridge(ModuleInitializationLevel p_level){
    //cleanup if needed
}

//--GDExtension エントリーポイント --
extern "C" {
    GDExtensionBool GDE_EXPORT godot_ros2_bridge_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization
    ){
        GDExtensionBinding::InitObject init_obj(p_get_proc_address,p_library,r_initialization);

        init_obj.register_initializer(initialize_godor_ros2_bridge);
        init_obj.register_terminator(uninitialize_godor_ros2_bridge);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        
        return init_obj.init();
    }
}