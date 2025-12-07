#ifndef CPP_LOGGING_SUITE_LOGGER_H
#define CPP_LOGGING_SUITE_LOGGER_H

#include "Color.h"

#include <print>
#include <string>
#include <iostream>
#include <mutex>
#include <chrono>

namespace logger {

    class Logger {
    public:
        enum class Level {
            None, Debug, Info, Success, Warning, Error, Fatal
        };

        struct Config {
            Level min_level = Level::None;
            bool show_timestamp = false;
            bool colors_enabled = true;
        };

    public:
        static const unsigned BARS = 50;

    public:
        static void set_config(Config config) {
            std::lock_guard lock(log_mutex);
            s_config = config;
        }

        static Config& get_config() {
            std::lock_guard lock(log_mutex);
            return s_config;
        }

        template<typename... Args>
        static void title(std::format_string<Args...> fmt, Args... args) {
            std::string bars(Logger::BARS / 2, '=');

            std::lock_guard lock(log_mutex);

            if (should_log(Level::Info)) {
                auto formatted = std::format(fmt, std::forward<Args>(args)...);
                if (s_config.colors_enabled) {
                    Color color_block(Color::Header);
                    std::println("\n{} {} {}", bars, formatted, bars);
                }
                else {
                    std::println("\n{} {} {}", bars, formatted, bars);
                }
            }
        }

        template<typename... Args>
        static void none(std::format_string<Args...> fmt, Args... args) {
            log(Level::None, Color::None, ' ', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void debug(std::format_string<Args...> fmt, Args... args) {
            log(Level::Debug, Color::Blue, 'D', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void info(std::format_string<Args...> fmt, Args... args) {
            log(Level::Info, Color::Info, 'i', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void success(std::format_string<Args...> fmt, Args... args) {
            log(Level::Success, Color::OkGreen, '+', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void warning(std::format_string<Args...> fmt, Args... args) {
            log(Level::Warning, Color::Warning, '!', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void error(std::format_string<Args...> fmt, Args... args) {
            log(Level::Error, Color::Fail, 'E', fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void fatal(std::format_string<Args...> fmt, Args... args) {
            log(Level::Fatal, Color::Fail, 'F', fmt, std::forward<Args>(args)...);
        }

        static void start_block(const std::string& _comment = "", const unsigned _bars = BARS) {
            std::string bars(_bars / 2, '-');
            std::lock_guard lock(log_mutex);
            std::println("\n{}{}{}", bars, _comment, bars);
        }

        static void end_block(const std::string& _comment = "", const unsigned _bars = BARS) {
            std::string bars(_bars / 2, '-');
            std::lock_guard lock(log_mutex);
            std::println("{}{}{}", bars, _comment, bars);
        }

    private:
        static bool should_log(Level level) {
            return static_cast<unsigned>(level) >= static_cast<unsigned>(s_config.min_level);
        }

        static std::string create_prefix(Level level, char symbol) {
            std::string prefix;

            if (s_config.show_timestamp) {
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                char time_buffer[20];
                std::tm tm;
                localtime_s(&tm, &time_t);
                std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &tm);
                prefix += std::format("{}", time_buffer);
            }

            prefix += std::format("[{}]", symbol);
            return prefix;
        }

        template<typename... Args>
        static void log(Level level, Color::Type color, char message_identificator, std::format_string<Args...> fmt, Args... args) {
            std::lock_guard lock(log_mutex);

            if (should_log(level) == false)
                return;

            auto formatted = std::format(fmt, std::forward<Args>(args)...);
            Color color_block(color);
            std::println("{} {}", create_prefix(level, message_identificator), formatted);
        }

    private:
        static inline std::mutex log_mutex;
        static inline Config s_config;
    };

    class Title {
    public:
        template<typename... Args>
        Title(std::format_string<Args...> fmt, Args... args) {
            Logger::title(fmt, std::forward<Args>(args)...);
        }
    };
}

#endif