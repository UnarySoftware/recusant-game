#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <shared/entity.hpp>

#include <shared/util.hpp>

using namespace godot;

namespace recusant
{
    bool Util::_initialized = false;
    
    bool Util::is_editor()
    {
        return (godot::Engine::get_singleton()->is_editor_hint() || !Util::_initialized);
    }

    bool Util::is_instance_valid(const godot::Object* p_object)
    {
        const uint64_t instance_id = p_object->get_instance_id();
        godot::Object* obj = godot::ObjectDB::get_instance(instance_id);
        return instance_id > 0 && obj != nullptr;
    }

    void Util::get_children_recursive(godot::Node* p_target, std::vector<godot::Node*>& p_children)
    {
        if (p_target == nullptr || !is_instance_valid(p_target))
        {
            return;
        }

        p_children.push_back(p_target);

        int32_t children = p_target->get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            godot::Node* p_node = p_target->get_child(i);

            if (p_node == nullptr || !is_instance_valid(p_node))
            {
                continue;
            }

            get_children_recursive(p_node, p_children);
        }
    }

    void Util::_get_files_recursively(godot::String p_path, godot::String p_file_extension, godot::PackedStringArray& p_files)
    {
        // Remove trailing slash if present
        if (p_path.ends_with("/") || p_path.ends_with("\\"))
        {
            p_path = p_path.left(p_path.length() - 1);
        }

        Ref<DirAccess> dir = DirAccess::open(p_path);

        if (dir == nullptr)
        {
            return;
        }

        dir->list_dir_begin();

        String file_name = dir->get_next();

        while (file_name != "")
        {
            if (dir->current_is_dir())
            {
                _get_files_recursively(dir->get_current_dir() + "/" + file_name, p_file_extension, p_files);
            }
            else
            {
                if (!p_file_extension.is_empty() && file_name.get_extension() != p_file_extension)
                {
                    file_name = dir->get_next();
                    continue;
                }
                p_files.append(dir->get_current_dir() + "/" + file_name);
            }

            file_name = dir->get_next();
        }
    }

    PackedStringArray Util::get_files_recursively(String p_path, String p_file_extension)
    {
        PackedStringArray result;
        _get_files_recursively(p_path, p_file_extension, result);
        return result;
    }

    Entity* Util::get_entity(Node* p_target)
    {
        if (p_target == nullptr)
        {
            return nullptr;
        }

        if (p_target->is_class(Entity::get_class_static()))
        {
            return Object::cast_to<Entity>(p_target);
        }

        while (true)
        {
            Node* parent = p_target->get_parent();
            if (parent == nullptr)
            {
                return nullptr;
            }
            p_target = parent;
            if (p_target->is_class(Entity::get_class_static()))
            {
                return Object::cast_to<Entity>(p_target);
            }
        }

        return nullptr;
    }
}
