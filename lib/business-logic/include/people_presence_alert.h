#pragma once

#include <stddef.h>
#include <stdint.h>

class MotionPresenceDetector
{
public:
    // Returns true when the presence state changes to present.
    bool update(const uint8_t *jpeg, size_t length, unsigned long now_ms);
    bool present() const { return present_; }

private:
    static constexpr size_t sample_count_ = 32;
    static constexpr unsigned active_frames_required_ = 3;
    static constexpr unsigned clear_frames_required_ = 12;
    static constexpr unsigned difference_threshold_ = 18;

    uint8_t previous_[sample_count_] = {};
    bool has_previous_ = false;
    bool present_ = false;
    unsigned active_frames_ = 0;
    unsigned clear_frames_ = 0;
};

class PeoplePresenceAlert
{
public:
    void process(const uint8_t *jpeg, size_t length, unsigned long now_ms);

private:
    MotionPresenceDetector detector_;
};
