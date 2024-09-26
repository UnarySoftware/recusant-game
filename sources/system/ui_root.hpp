#pragma once

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class UiState;

    class UiRoot : public System
    {
        GD_CLASS(UiRoot, System);

    private:

        std::vector<UiState*> _state_vec;
        std::unordered_map<godot::String, UiState*, godot::hash::String> _state_map;
        
        DEFINE_SINGLETON();

    protected:
        
        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        void initialize() override;
        void deinitialize() override;
    };
}
