#include <system/units/steam_callback.hpp>

using namespace godot;

namespace recusant
{
    void SteamCallback::_bind_methods()
    {
    }

    void SteamCallback::set(CallbackType p_type, uint64_t *p_pointer)
    {
        _type = p_type;
        _pointer = p_pointer;
    }

    SteamCallback::CallbackType SteamCallback::get_type()
    {
        return _type;
    }
}
