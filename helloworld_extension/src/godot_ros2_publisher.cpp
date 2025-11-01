#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "register_types.h"

using namespace godot;

class HelloWorld : public Object{
    GDCLASS(HelloWorld,Object);

    protected:
    static void _bind_methods(){
        ClassDB::bind_method(D_METHOD("say_hello"),&HelloWorld::say_hello);
    }

    public:
    void say_hello(){
        UtilityFunctions::print("Hello from C++ Extentions made from daichan06");
    }
};


//--登録処理--
void initialize_myextension(ModuleInitializationLevel p_level){
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
    ClassDB::register_class<HelloWorld>();
}

void uninitialize_myextension(ModuleInitializationLevel p_level){
    //cleanup if needed
}

//--GDExtension エントリーポイント --
extern "C" {
    GDExtensionBool GDE_EXPORT myextension_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization
    ){
        GDExtensionBinding::InitObject init_obj(p_get_proc_address,p_library,r_initialization);

        init_obj.register_initializer(initialize_myextension);
        init_obj.register_terminator(uninitialize_myextension);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        
        return init_obj.init();
    }
}