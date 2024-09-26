#include <godot_cpp/classes/label3d.hpp>

#include <dynamic/player/label.hpp>

using namespace godot;

namespace recusant
{
    void PlayerLabel::_bind_methods()
    {
        BIND_PROPERTY_PATH(Label3D, label);
    }

    void PlayerLabel::_ready()
    {
        RUNTIME_ONLY();
    }

    void PlayerLabel::on_authority_changed()
    {
        if (is_multiplayer_authority())
        {
            _label->set_visible(false);
        }
        else
        {
            _label->set_visible(true);
            _label->set_text(String::num_int64(get_multiplayer_authority()));
        }
    }
}
