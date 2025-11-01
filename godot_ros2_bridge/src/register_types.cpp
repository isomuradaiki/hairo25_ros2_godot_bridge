//#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>


#include <vector>
#include <memory>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>



#include "ros2_publisher.h"
#include "ros2_subscriber.h"

#include "ros2_bridge.h"

using namespace godot;

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