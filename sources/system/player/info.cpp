#include <system/player/info.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(Info);

    void Info::_ready()
    {
        ASSIGN_SINGLETON();
    }

    void Info::_bind_methods()
    {
        BIND_SINGLETON();
    }
}
