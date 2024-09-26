#pragma once

#include <ui/ui_state.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class EscapeState : public UiState
    {
        GD_CLASS(EscapeState, UiState);

    private:

        DEFINE_SINGLETON();

    protected:
        
        static void _bind_methods();

    public:

        IMPL_SINGLETON();
        
        void initialize() override;
        void deinitialize() override;

    };
}
