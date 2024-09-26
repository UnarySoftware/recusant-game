#pragma once

#include <shared/components/interactable.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class DrillResources;

    class DrillInteractable : public Interactable
    {
        GD_CLASS(DrillInteractable, Interactable);

    private:

        DrillResources* _resources = nullptr;

    protected:

        static void _bind_methods();

    public:
        
        void _ready() override;

        void get_resources(Entity* p_taker);
    };
}
