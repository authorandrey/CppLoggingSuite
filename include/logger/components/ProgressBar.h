#ifndef CPP_LOGGING_SUITE_PROGRESS_BAR_H
#define CPP_LOGGING_SUITE_PROGRESS_BAR_H

#include "core/Logger.h"
#include <vector>

namespace logger {
    class ProgressBar {
    public:
        explicit ProgressBar(unsigned total = 100, const std::string& description = "")
            : m_total(total),
            m_current(0),
            m_bar_width(50),
            m_description(description),
            m_fill("="),
            m_remainder(" "),
            m_status("") {
        }

        void finish(const std::string& status = "Finished", std::ostream& os = std::cout) {
            m_status = status;
            m_current = m_total;
            write_progress(os);
            os << std::endl;
        }

        void set_total(unsigned total) { m_total = total; }
        void set_description(const std::string& description) { m_description = description; }
        void set_bar_width(unsigned width) { m_bar_width = width; }
        void fill_bar_with(const std::string& chars) { m_fill = chars; }
        void fill_remainder_with(const std::string& chars) { m_remainder = chars; }
        void set_status(const std::string& status) { m_status = status; }

        void update(unsigned current, std::ostream& os = std::cout) {
            m_current = current;
            write_progress(os);
        }

        void increment(unsigned n = 1, std::ostream& os = std::cout) {
            m_current += n;
            if (m_current > m_total) m_current = m_total;
            write_progress(os);
        }

        void write_progress(std::ostream& os = std::cout) const {
            if (m_total == 0) return;

            float progress = static_cast<float>(m_current) / m_total;
            size_t pos = static_cast<size_t>(progress * m_bar_width);

            std::string bar;
            for (size_t i = 0; i < m_bar_width; ++i) {
                if (i < pos) bar += m_fill;
                else if (i == pos) bar += ">";
                else bar += m_remainder;
            }

            auto output = std::format("{} [{}] {:3.0f}%{}",
                m_description, bar, progress * 100.0,
                m_status.empty() ? "" : " " + m_status
            );

            os << "\r" << output << std::flush;
        }

    private:
        unsigned m_total;
        unsigned m_current;
        unsigned m_bar_width;
        std::string m_description;
        std::string m_fill;
        std::string m_remainder;
        std::string m_status;
    };
}

#endif