#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <shared/util.hpp>

#define GET_CALLBACK_PTR(p_callback, p_callback_enum)                                                           \
    auto callback = p_callback->get_pointer<p_callback_enum##_t>(SteamCallback::CallbackType::p_callback_enum); \
    if (callback == nullptr)                                                                                    \
    {                                                                                                           \
        return;                                                                                                 \
    }

namespace recusant
{
    class SteamCallback : public godot::RefCounted
    {
        GD_CLASS(SteamCallback, RefCounted);

    public:
        enum CallbackType
        {
            None,
            SteamNetConnectionStatusChangedCallback,
        };

    private:

        CallbackType _type = CallbackType::None;
        uint64_t* _pointer = nullptr;

    protected:
        static void _bind_methods();

    public:
        void set(CallbackType p_type, uint64_t* p_pointer);
        CallbackType get_type();

        template <class T>
        T* get_pointer(CallbackType p_type)
        {
            if (_type == p_type)
            {
                return reinterpret_cast<T*>(_pointer);
            }
            return nullptr;
        }
    };
}
