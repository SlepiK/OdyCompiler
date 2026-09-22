#ifndef SIERRA_ODYCOMPILER_TIMER_HPP
#define SIERRA_ODYCOMPILER_TIMER_HPP

#include <chrono>

namespace sierra::odycompiler {
    class Timer {
    public:
        Timer() = default;
        ~Timer() = default;

        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer(Timer&&) = default;
        Timer& operator=(Timer&&) = default;

        void start();
        void stop() const;
    private:
        std::chrono::steady_clock::time_point mStart;
    };
}

#endif //SIERRA_ODYCOMPILER_TIMER_HPP