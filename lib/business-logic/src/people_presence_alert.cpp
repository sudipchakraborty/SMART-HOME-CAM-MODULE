#include "people_presence_alert.h"

#include <Arduino.h>

bool MotionPresenceDetector::update(const uint8_t *jpeg, size_t length, unsigned long)
{
    if (jpeg == nullptr || length < sample_count_)
        return false;

    uint8_t current[sample_count_];
    unsigned difference = 0;
    for (size_t i = 0; i < sample_count_; ++i)
    {
        const size_t offset = (i * (length - 1)) / (sample_count_ - 1);
        current[i] = jpeg[offset];
        if (has_previous_)
        {
            const int delta = static_cast<int>(current[i]) - previous_[i];
            difference += static_cast<unsigned>(delta < 0 ? -delta : delta);
        }
    }

    for (size_t i = 0; i < sample_count_; ++i)
        previous_[i] = current[i];

    if (!has_previous_)
    {
        has_previous_ = true;
        return false;
    }

    const bool active = (difference / sample_count_) >= difference_threshold_;
    if (active)
    {
        clear_frames_ = 0;
        if (active_frames_ < active_frames_required_)
            ++active_frames_;
    }
    else
    {
        active_frames_ = 0;
        if (clear_frames_ < clear_frames_required_)
            ++clear_frames_;
    }

    if (!present_ && active_frames_ >= active_frames_required_)
    {
        present_ = true;
        return true;
    }
    if (present_ && clear_frames_ >= clear_frames_required_)
        present_ = false;

    return false;
}

void PeoplePresenceAlert::process(const uint8_t *jpeg, size_t length, unsigned long now_ms)
{
    if (detector_.update(jpeg, length, now_ms))
        Serial.println("people found");
}
