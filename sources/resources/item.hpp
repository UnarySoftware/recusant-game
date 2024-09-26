#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture.hpp>

#include <shared/util.hpp>

namespace recusant
{
    class Item : public godot::Resource
    {
        GD_CLASS(Item, Resource);

    private:

        float _weight = 10.0f;
        float _price = 10.0f;
        int _size_x = 1;
        int _size_y = 1;
        godot::Ref<godot::Texture> _texture;

    protected:

        static void _bind_methods()
        {
            BIND_PROPERTY(godot::Variant::FLOAT, weight);
            BIND_PROPERTY(godot::Variant::FLOAT, price);
            BIND_PROPERTY(godot::Variant::INT, size_x);
            BIND_PROPERTY(godot::Variant::INT, size_y);
            BIND_PROPERTY_PATH_RESOURCE(Texture, texture);
        };

    public:

        IMPL_PROPERTY(float, weight);
        IMPL_PROPERTY(float, price);
        IMPL_PROPERTY(int, size_x);
        IMPL_PROPERTY(int, size_y);
        IMPL_PROPERTY(godot::Ref<godot::Texture>, texture);

    };
}
