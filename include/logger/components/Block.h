#ifndef CPP_LOGGING_SUITE_BLOCK_H
#define CPP_LOGGING_SUITE_BLOCK_H

#include "core/Logger.h"
#include <unordered_map>

namespace logger {
    class Block {
    public:
        template<typename... Args>
        Block(std::format_string<Args...> fmt, Args... args) {
            auto formatted = std::format(fmt, std::forward<Args>(args)...);
            m_string_size = formatted.size();
            m_level = s_block_level++;
            Logger::start_block(formatted, Logger::BARS + m_level * 2);
        }

        ~Block() {
            Logger::end_block("", Logger::BARS + m_level * 2 + m_string_size);
            s_block_level--;
        }

    private:
        size_t m_string_size;
        size_t m_level;
        static inline size_t s_block_level = 0;
    };

    class NonNestableBlock {
    public:
        template<typename... Args>
        NonNestableBlock(std::format_string<Args...> fmt, Args... args)
            : m_formatted(std::format(fmt, std::forward<Args>(args)...)) {

            std::lock_guard lock(s_mutex);

            if (s_active_block && !s_active_block->m_closed) {
                s_active_block->close_impl();
            }

            s_active_block = this;

            open_impl();
        }

        ~NonNestableBlock() {
            std::lock_guard lock(s_mutex);
            if (s_active_block == this && !m_closed) {
                close_impl();
            }
            if (s_active_block == this) {
                s_active_block = nullptr;
            }
        }

        NonNestableBlock(const NonNestableBlock&) = delete;
        NonNestableBlock& operator=(const NonNestableBlock&) = delete;
        NonNestableBlock(NonNestableBlock&&) = delete;
        NonNestableBlock& operator=(NonNestableBlock&&) = delete;

        void close() {
            std::lock_guard lock(s_mutex);
            if (!m_closed) {
                close_impl();
                if (s_active_block == this) {
                    s_active_block = nullptr;
                }
            }
        }

        bool is_active() const {
            std::lock_guard lock(s_mutex);
            return s_active_block == this && !m_closed;
        }

        const std::string& text() const { return m_formatted; }

    private:
        void open_impl() {
            Logger::start_block(m_formatted, Logger::BARS);
            m_closed = false;
        }

        void close_impl() {
            Logger::end_block("", Logger::BARS + m_formatted.size());
            m_closed = true;
        }

    private:
        std::string m_formatted;
        bool m_closed = false;

        static inline std::mutex s_mutex;
        static inline NonNestableBlock* s_active_block = nullptr;
    };
}

#endif
