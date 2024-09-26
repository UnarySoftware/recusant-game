#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace recusant
{
    class MotionGenerator : public godot::RefCounted
    {
        GD_CLASS(MotionGenerator, RefCounted);

    private:
        struct MotionGeneratorFrame
        {
            float interpolation_time = 0.0f;
            godot::Vector3 position;
            godot::Vector3 velocity;
            godot::Quaternion rotation;
            godot::Vector3 angular_velocity;
        };

        static MotionGeneratorFrame _default_frame;
        
        // It's basically a frame buffer length defined in seconds.
        // Values above sync message send interval increase resistance to bad network conditions.
        // Values below sync message send interval compensate the latency by using extrapolation.
        float _interpolation_latency = 0.08f;
        // Smoothly corrects position and rotation extrapolation errors.
        float _error_correction_speed = 10.0f;
        // Smoothly corrects time drift back into correct time frame.
        float _time_correction_speed = 0.75f;
        // Sometimes extrapolation may not be desired at all because of overshooting.
        // Make sure to use interpolation_latency way higher than send interval if you enable this setting.
        bool _disable_extrapolation = true;
        // Gets playback time between current and the next frame.
        float _playback_time = 0.0f;
        // Gets current time drift.
        float _time_drift = 0.0f;
        std::vector<MotionGeneratorFrame> _frames;
        // Gets current position drift.
        godot::Vector3 _extrapolation_position_drift = godot::zero::Vector3;
        // Gets current rotation drift.
        godot::Quaternion _extrapolation_rotation_drift = godot::zero::Quaternion;

        // Gets last received frame.
        MotionGeneratorFrame& _get_last_received_frame();
        // Gets frame you're currently interpolating from.
        MotionGeneratorFrame& _get_current_frame();
        
    protected:
        static void _bind_methods() {};

    public:
        // Used to identify if it's currently playing back through frames or predicting the future.
        bool is_extrapolating();
        // Indicates if the frame buffer is not empty (received any frames).
        bool has_frames();
        // Same as get_position(), but doesn't smoothly correct extrapolation drift when new frame arrives.
        godot::Vector3 get_position_no_error_correction();
        // Gets position at current playback time.
        godot::Vector3 get_position();
        // Gets velocity at current playback time.
        godot::Vector3 get_velocity();
        // Same as get_rotation(), but doesn't smoothly correct extrapolation drift when new frame arrives.
        godot::Quaternion get_rotation_no_error_correction();
        // Gets rotation at current playback time.
        godot::Quaternion get_rotation();
        // Gets angular velocity at current playback time.
        godot::Vector3 get_angular_velocity();
        
        // Gets the rotation difference between two quaternions.
        // Basically output is "p_to_rotation - p_from_rotation", but in quaternion math.
        godot::Quaternion get_rotation_difference(godot::Quaternion p_from_rotation, godot::Quaternion p_to_rotation);
        // Converts angle values from 0 to 360 degree range into -180 to 180 degree range.
        float format_euler_angle(float p_angle);
        // Converts vector rotation values from 0 to 360 degree range into -180 to 180 degree range.
        godot::Vector3 format_euler_rotation(godot::Vector3 p_euler_rotation);
        // Progress the playback further by deltaTime.
        // delta - Time in seconds.
        void update(double p_delta);
        // Adds a new keyframe to the buffer.
        void add_frame(float p_interpolation_time, godot::Vector3 p_position, godot::Quaternion p_rotation);
    };
}
