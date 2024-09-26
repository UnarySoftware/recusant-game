#include <shared/util.hpp>
#include <system/logger.hpp>

#include <shared/units/motion_generator.hpp>

using namespace godot;

namespace recusant
{
    MotionGenerator::MotionGeneratorFrame MotionGenerator::_default_frame;

    MotionGenerator::MotionGeneratorFrame& MotionGenerator::_get_last_received_frame()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access last received frame in an empty buffer. Zero vector returned.";
            return _default_frame;
        }
        return _frames[_frames.size() - 1];
    }

    MotionGenerator::MotionGeneratorFrame& MotionGenerator::_get_current_frame()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access last received frame in an empty buffer. Zero vector returned.";
            return _default_frame;
        }
        return _frames[0];
    }

    bool MotionGenerator::is_extrapolating()
    {
        return _frames.size() == 1;
    }

    bool MotionGenerator::has_frames()
    {
        return _frames.size() != 0;
    }

    Vector3 MotionGenerator::get_position_no_error_correction()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access position in an empty buffer. Zero vector returned.";
            return godot::zero::Vector3;
        }

        if (_disable_extrapolation && is_extrapolating())
        {
            return _frames[0].position;
        }

        return _frames[0].position + _frames[0].velocity * _playback_time;
    }

    Vector3 MotionGenerator::get_position()
    {
        return get_position_no_error_correction() + _extrapolation_position_drift;
    }

    Vector3 MotionGenerator::get_velocity()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access velocity in an empty buffer. Zero vector returned.";
            return godot::zero::Vector3;
        }

        return _frames[0].velocity;
    }

    Quaternion MotionGenerator::get_rotation_no_error_correction()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access rotation in an empty buffer. Zero rotation returned.";
            return godot::zero::Quaternion;
        }

        if (_disable_extrapolation && is_extrapolating())
        {
            return _frames[0].rotation;
        }

        return _frames[0].rotation * Quaternion(_frames[0].angular_velocity * _playback_time);
    }

    Quaternion MotionGenerator::get_rotation()
    {
        return get_rotation_no_error_correction() * _extrapolation_rotation_drift;
    }

    Vector3 MotionGenerator::get_angular_velocity()
    {
        if (!has_frames())
        {
            Log::warning() << "Trying to access angular velocity in an empty buffer. Zero vector returned.";
            return godot::zero::Vector3;
        }

        return _frames[0].angular_velocity;
    }

    Quaternion MotionGenerator::get_rotation_difference(Quaternion p_from_rotation, Quaternion p_to_rotation)
    {
        Quaternion result(p_from_rotation);
        result = result.inverse();
        return result * p_to_rotation;
    }

    float MotionGenerator::format_euler_angle(float p_angle)
    {
        return p_angle > 180.0f ? p_angle - 360.0f : p_angle;
    }

    Vector3 MotionGenerator::format_euler_rotation(Vector3 p_euler_rotation)
    {
        return Vector3(format_euler_angle(p_euler_rotation.x), format_euler_angle(p_euler_rotation.y), format_euler_angle(p_euler_rotation.z));
    }

    void MotionGenerator::update(double p_delta)
    {
        if (_frames.size() < 1)
        {
            Log::warning() << "Trying to update playback in an empty buffer.";
            return;
        }

        if (p_delta > 0.0f)
        {
            // Smoothly correct time drift.
            float time_drift_correction = -Math::lerp(0.0f, _time_drift, _time_correction_speed * p_delta);
            // Add the time drift correction to delta time so it properly affects everything else.
            p_delta += time_drift_correction;
            _time_drift += time_drift_correction;
            _playback_time += p_delta;

            // Smoothly correct extrapolation errors.
            _extrapolation_position_drift = _extrapolation_position_drift.lerp(godot::zero::Vector3, _error_correction_speed * p_delta);
            _extrapolation_rotation_drift = _extrapolation_rotation_drift.slerp(godot::zero::Quaternion, _error_correction_speed * p_delta);
        }

        // Remove old keyframes.
        while (_frames.size() > 1 && _playback_time >= _frames[1].interpolation_time)
        {
            // If you're going through an instant keyframe, nullify drifting because it's a teleport keyframe.
            if (_frames[1].interpolation_time == 0.0f)
            {
                _extrapolation_position_drift = godot::zero::Vector3;
                _extrapolation_rotation_drift = godot::zero::Quaternion;
            }

            _playback_time -= _frames[1].interpolation_time;
            _frames.erase(_frames.begin());
        }
    }

    void MotionGenerator::add_frame(float p_interpolation_time, Vector3 p_position, Quaternion p_rotation)
    {
        // Prevent long first frame if some keyframes were skipped before the first frame.
        if (_frames.size() < 1)
        {
            p_interpolation_time = Math::max(_interpolation_latency, 0.01f);
        }

        // Calculate time drift.
        float time_till_frame_buffer_end = p_interpolation_time - _playback_time;
        for (int i = 1; i < _frames.size(); i++)
        {
            time_till_frame_buffer_end += _frames[i].interpolation_time;
        }

        _time_drift = _interpolation_latency - time_till_frame_buffer_end;

        // Inserting empty keyframe the buffer is empty (get_last_received_frame() doesn't exist).
        if (_frames.size() < 1)
        {
            MotionGeneratorFrame fake_frame;
            fake_frame.interpolation_time = 0.0f;
            fake_frame.position = p_position;
            fake_frame.velocity = godot::zero::Vector3;
            fake_frame.rotation = p_rotation;
            fake_frame.angular_velocity = godot::zero::Vector3;
            _frames.push_back(fake_frame);
        }

        Vector3 position_before_new_frame = get_position();
        Quaternion rotation_before_new_frame = get_rotation();

        // Add the keyframe.
        MotionGeneratorFrame& last_received_frame = _get_last_received_frame();
        Vector3 calculated_velocity = p_interpolation_time > 0.0f ? (p_position - last_received_frame.position) / p_interpolation_time : godot::zero::Vector3;
        Quaternion calculated_rotation_difference = get_rotation_difference(last_received_frame.rotation, p_rotation);
        Vector3 calculated_angular_velocity = p_interpolation_time > 0.0f ? format_euler_rotation(calculated_rotation_difference.get_euler()) / p_interpolation_time : godot::zero::Vector3;

        MotionGeneratorFrame frame;
        frame.interpolation_time = p_interpolation_time;
        frame.position = p_position;
        frame.velocity = calculated_velocity;
        frame.rotation = p_rotation;
        frame.angular_velocity = calculated_angular_velocity;
        _frames.push_back(frame);

        // Set previous keyframe velocity to match the new position.
        last_received_frame.velocity = calculated_velocity;
        last_received_frame.angular_velocity = calculated_angular_velocity;
        _frames[_frames.size() - 2] = last_received_frame;

        // Get onto a new frame if needed.
        update(0.0f);

        // Calculate drift.
        Vector3 position_after_new_frame = get_position_no_error_correction();
        Quaternion rotation_after_new_frame = get_rotation_no_error_correction();

        _extrapolation_position_drift = position_before_new_frame - position_after_new_frame;
        _extrapolation_rotation_drift = get_rotation_difference(rotation_after_new_frame, rotation_before_new_frame);
    }
}
