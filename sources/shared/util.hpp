#pragma once

#include <vector>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/variant/string.hpp>

// TODO Cleanup this file

#define BIND_RPC(p_method, p_authority, p_transfer_mode, p_call_local) \
    do { \
    Dictionary __##p_method##_bind_rpc; \
    __##p_method##_bind_rpc["rpc_mode"] = p_authority; \
    __##p_method##_bind_rpc["transfer_mode"] = p_transfer_mode; \
    __##p_method##_bind_rpc["call_local"] = p_call_local; \
    __##p_method##_bind_rpc["channel"] = 0; \
    rpc_config(#p_method, __##p_method##_bind_rpc); \
    } while (0)

#define MAX_TPS 20.0f // 20 times per second
#define MIN_TPS 0.2f // Once per 5 seconds
#define SLEEP_TPS 0.00001f // Once in 100000 seconds (27.7 hrs)

#define RPC_EVERYONE rpc
#define RPC_TARGET rpc_id
#define SERVER_PEER 1

namespace godot
{
    class Node;

    namespace zero
    {
        static const godot::Vector3 Vector3(0.0f, 0.0f, 0.0f);
        static const godot::Quaternion Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        static const godot::AABB AABB(Vector3, Vector3);
    }
    namespace one
    {
        static const godot::Vector3 Vector3(1.0f, 1.0f, 1.0f);
    }
    namespace base
    {
        static const godot::Basis Basis(godot::Vector3(1.0f, 0.0f, 0.0f), godot::Vector3(0.0f, 1.0f, 0.0f), godot::Vector3(0.0f, 0.0f, 1.0f));
    }
    class hash
    {

    public:

        struct String
        {
            std::size_t operator()(const godot::String& h) const { return h.hash(); }
        };

    };
}

#define NAMEOF(p_x) #p_x

// Do not use this in places besides macros, PLEASE!!! We need code readability!!!
#define __CONCAT(x, y) #x "" #y
#define __TOKEN_PASTE(x, y) x##y
#define __CAT(x,y) __TOKEN_PASTE(x,y)

// Generic

#define GD_CLASS(p_class, p_inherits) \
    typedef p_inherits Super; \
    typedef p_class CurrentClass; \
    GDCLASS(p_class, p_inherits) \
    struct __CAT(semicolon_place, __LINE__)

#define IMPL_PROPERTY_NONCONST(p_type, p_name) \
    void set_##p_name(p_type p_##p_name) { \
    _##p_name = p_##p_name; } \
    p_type get_##p_name() { \
    return _##p_name; } \
    struct __CAT(semicolon_place, __LINE__)

#define IMPL_PROPERTY(p_type, p_name) \
    void set_##p_name(const p_type p_##p_name) { \
    _##p_name = p_##p_name; } \
    p_type get_##p_name() const { \
    return _##p_name; } \
    struct __CAT(semicolon_place, __LINE__)

#define DEFINE_SINGLETON() \
    static CurrentClass* _singleton; \
    struct __CAT(semicolon_place, __LINE__)

#define DECLARE_SINGLETON(p_type) \
    p_type* p_type::_singleton; \
    struct __CAT(semicolon_place, __LINE__)

#define ASSIGN_SINGLETON() \
    do { \
    _singleton = this; \
    } while (0)

#define UNASSIGN_SINGLETON() \
    do { \
    if (_singleton != nullptr) \
    _singleton = nullptr; \
    } while (0)

#define IMPL_SINGLETON() \
    static CurrentClass* get_singleton() { return godot::Object::cast_to<CurrentClass>(_singleton); } \
    struct __CAT(semicolon_place, __LINE__)

#define BIND_PROPERTY(p_type, p_name) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(get_, p_name)), &CurrentClass::get_##p_name); \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(set_, p_name)), &CurrentClass::set_##p_name); \
    ADD_PROPERTY(godot::PropertyInfo(p_type, #p_name), __CONCAT(set_, p_name), __CONCAT(get_, p_name)); \
    } while (0)

#define BIND_PROPERTY_HINTED(p_type, p_name, p_hint, p_hint_string) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(get_, p_name)), &CurrentClass::get_##p_name); \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(set_, p_name)), &CurrentClass::set_##p_name); \
    ADD_PROPERTY(godot::PropertyInfo(p_type, #p_name, p_hint, p_hint_string), __CONCAT(set_, p_name), __CONCAT(get_, p_name)); \
    } while (0)

#define BIND_PROPERTY_HINTED_USAGE(p_type, p_name, p_hint, p_hint_string, p_usage) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(get_, p_name)), &CurrentClass::get_##p_name); \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(set_, p_name)), &CurrentClass::set_##p_name); \
    ADD_PROPERTY(godot::PropertyInfo(p_type, #p_name, p_hint, p_hint_string, p_usage), __CONCAT(set_, p_name), __CONCAT(get_, p_name)); \
    } while (0)

#define BIND_PROPERTY_VIRTUAL(p_name) \
    do { \
    BIND_VIRTUAL_METHOD(CurrentClass, get_##p_name); \
    BIND_VIRTUAL_METHOD(CurrentClass, set_##p_name); \
    } while (0)

// Property path

#define BIND_PROPERTY_PATH(p_type, p_name) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(get_, p_name)), &CurrentClass::get_##p_name); \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(set_, p_name)), &CurrentClass::set_##p_name); \
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, #p_name, PROPERTY_HINT_NODE_TYPE, #p_type), __CONCAT(set_, p_name), __CONCAT(get_, p_name)); \
    } while (0)

#define BIND_PROPERTY_PATH_RESOURCE(p_type, p_name) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(get_, p_name)), &CurrentClass::get_##p_name); \
    godot::ClassDB::bind_method(godot::D_METHOD(__CONCAT(set_, p_name)), &CurrentClass::set_##p_name); \
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, #p_name, PROPERTY_HINT_RESOURCE_TYPE, #p_type), __CONCAT(set_, p_name), __CONCAT(get_, p_name)); \
    } while (0)

// Debug only methods

#ifdef DEBUG_ENABLED
#define IS_EDITOR Util::is_editor()
#define IS_RUNTIME !(IS_EDITOR)
#define RUNTIME_ONLY(v) \
do { \
if (IS_EDITOR) { return v; } \
} while (0)
#define EDITOR_ONLY(v) \
do { \
if (IS_RUNTIME) { return v; } \
} while (0)
// TODO: Add GODOT_PROFILING_FUNCTION support https://github.com/godotengine/godot-cpp/issues/1032
#else
#define IS_EDITOR false
#define IS_RUNTIME true
#define RUNTIME_ONLY(v) 
#define EDITOR_ONLY(v) return v;
#endif

// Method binding

#define BIND_METHOD(p_name) \
    do { \
    godot::ClassDB::bind_method(godot::D_METHOD(#p_name), &CurrentClass::p_name); \
    } while (0)

#define BIND_SINGLETON() \
    do { \
    godot::ClassDB::bind_static_method(CurrentClass::get_class_static(), D_METHOD("get_singleton"), &CurrentClass::get_singleton); \
    } while (0)

// Util class

namespace recusant
{
    class Entity;

    class Util
    {
    private:
        static void _get_files_recursively(godot::String p_path, godot::String p_file_extension, godot::PackedStringArray& p_files);

    public:
        static bool _initialized;
        static bool is_editor();
        static bool is_instance_valid(const godot::Object* p_object);
        static void get_children_recursive(godot::Node* p_target, std::vector<godot::Node*>& p_children);
        static godot::PackedStringArray get_files_recursively(godot::String p_path, godot::String p_file_extension = "");
        static Entity* get_entity(godot::Node* p_target);
    };
}
