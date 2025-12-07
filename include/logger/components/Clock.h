#ifndef CPP_LOGGING_SUITE_CLOCK_H
#define CPP_LOGGING_SUITE_CLOCK_H

#include "core/Logger.h"

#include <chrono>

namespace logger {
    class Clock {
    public:
        enum TimeUnit {
            Seconds,
            MilliSeconds,
            MicroSeconds,
            NanoSeconds,
        };

    public:
        Clock() : m_start(std::chrono::high_resolution_clock::now()) {}

        void elapsed(const std::string& _message, TimeUnit time_unit = TimeUnit::MilliSeconds) const {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = end - m_start;
            auto [time, unit] = getElapsed(duration, time_unit);
            Logger::debug("{}: {}{}", _message, time, unit);
        }

    private:
        static std::pair<double, std::string_view> getElapsed(std::chrono::nanoseconds duration, TimeUnit time_unit = TimeUnit::MilliSeconds) {
            switch (time_unit) {
            case TimeUnit::Seconds:
                return {
                    std::chrono::duration<double>(duration).count(),
                    "s"
                };
            case TimeUnit::MilliSeconds:
                return {
                    std::chrono::duration<double, std::milli>(duration).count(),
                    "ms"
                };
            case TimeUnit::MicroSeconds:
                return {
                    std::chrono::duration<double, std::micro>(duration).count(),
                    "mcs"
                };
            case TimeUnit::NanoSeconds:
                return {
                    std::chrono::duration<double, std::nano>(duration).count(),
                    "ns"
                };
            default:
                throw std::runtime_error("Failed to find appropriate time format");
            }
        }

    private:
        std::chrono::steady_clock::time_point m_start;
    };
}

#endif