#include <shared/util.hpp>

#include <shared/units/sleep_block.hpp>

using namespace godot;

namespace recusant
{
    const double SleepBlock::_offset_step = 0.01;
    const double SleepBlock::_max_offset = 0.3;
    double SleepBlock::_current_step = 0.0;

    void SleepBlock::_bind_methods()
    {
        ADD_SIGNAL(MethodInfo("on_active"));
        ADD_SIGNAL(MethodInfo("on_sleep"));
    }

    void SleepBlock::set(double p_tps)
    {
        if (p_tps == 0.0)
        {
            _type = Type::Disabled;
            return;
        }
        _timeout = 1.0 / p_tps;
        _timer = _timeout + 1.0;
        _type = Type::Singular;
        _current_step += _offset_step;
        if (_current_step >= _max_offset)
        {
            _current_step = 0.0;
        }
        _timer += _current_step;
    }

    void SleepBlock::set(double p_span, double p_timeout)
    {
        if (p_span == 0.0 && p_timeout == 0.0)
        {
            _type = Type::Disabled;
            return;
        }
        _span = p_timeout;
        _timeout = p_span;
        _type = Type::Range;
        _current_step += _offset_step;
        if (_current_step >= _max_offset)
        {
            _current_step = 0.0;
        }
        _timer += _current_step;
    }

    bool SleepBlock::update(double p_delta)
    {
        _timer += p_delta;

        // Acting as sleep with range
        if (_type == Type::Disabled)
        {
            return false;
        }
        else if (_type == Type::Range)
        {
            if (_sleeping)
            {
                if (_timer >= _timeout)
                {
                    _timer = 0.0;
                    emit_signal(on_active);
                    _sleeping = false;
                    return true;
                }
            }
            else
            {
                if (_timer >= _span)
                {
                    _timer = 0.0;
                    emit_signal(on_sleep);
                    _sleeping = true;
                }
            }
            return false;
        }
        // Acting as a singular sleep
        else
        {
            if (_timer >= _timeout)
            {
                _timer = 0.0;
                return true;
            }
            return false;
        }
    }

}
