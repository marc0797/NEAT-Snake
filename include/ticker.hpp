#include <iostream>
#include <chrono>

class Ticker {
public:
    Ticker(double target_fps) {
        setFrameRate(target_fps);
        last_tick = std::chrono::high_resolution_clock::now();
    }

    void setFrameRate(double target_fps) {
        if (target_fps <= 0) {
            throw std::invalid_argument("FPS must be greater than 0");
        }
        target_frame_duration = std::chrono::duration<double>(1.0 / target_fps);
    }

    bool tick() {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = now - last_tick;

        if (elapsed >= target_frame_duration) {
            last_tick = now;
            return true;
        } else {
            auto ticks_passed = static_cast<int>(elapsed / target_frame_duration);
            if (ticks_passed > 0) {
                last_tick = now;
                return ticks_passed;
            }
            return false;
        }
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> last_tick;
    std::chrono::duration<double> target_frame_duration;
};