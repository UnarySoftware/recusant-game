#pragma once

#include <shared/synchronizers/base_synchronizer.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class AuthoritySynchronizer : public BaseSynchronizer
    {
        GD_CLASS(AuthoritySynchronizer, BaseSynchronizer);

    private:

        int32_t _authority = SERVER_PEER;

    protected:
        
        static void _bind_methods();

    public:

        void set_authority(const int32_t p_authority);
        int32_t get_authority() const;

        void _ready() override;
        void set_sleep(bool p_value) override;
    };
}
