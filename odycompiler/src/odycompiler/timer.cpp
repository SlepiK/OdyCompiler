#include <odycompiler/timer.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler {
    void Timer::start() {
        mStart = std::chrono::steady_clock::now();
    }

    void Timer::stop() const {
        const auto end = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - mStart);
        spdlog::info("Used time for compilation: {} µs", duration.count());
    }
}
