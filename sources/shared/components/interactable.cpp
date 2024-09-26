#include <shared/components/interactable.hpp>

using namespace godot;

namespace recusant
{
    void Interactable::add_option(const godot::String &p_option_name, const godot::Callable &p_callable)
    {
        _options[p_option_name] = p_callable;
    }

    void Interactable::clear_option()
    {
        _options.clear();
    }

    const HashMap<godot::String, godot::Callable> Interactable::get_options()
    {
        return _options;
    }
}
