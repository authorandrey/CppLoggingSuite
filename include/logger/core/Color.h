#ifndef CPP_LOGGING_SUITE_COLOR_H
#define CPP_LOGGING_SUITE_COLOR_H

#include <iostream>

namespace logger {
    namespace bcolors {
        constexpr const char* FAIL = "\033[91m";
        constexpr const char* OKGREEN = "\033[92m";
        constexpr const char* INFO = "\033[93m";
        constexpr const char* OKCYAN = "\033[96m";
        constexpr const char* ENDC = "\033[0m";
        constexpr const char* HEADER = "\033[95m";
        constexpr const char* WARNING = "\033[93m";
        constexpr const char* BOLD = "\033[1m";
        constexpr const char* UNDERLINE = "\033[4m";
        constexpr const char* BLUE = "\033[94m";
        constexpr const char* MAGENTA = "\033[95m";
    };

    class Color {
    public:
        enum Type {
            None, Fail, OkGreen, Info, OkCyan, Header, Warning, Bold, Underline, Blue, Magenta
        };

    public:
        Color(Type color, std::ostream& os = std::cout) : m_os(os) {
            apply_color(color);
        }

        ~Color() { std::cout << bcolors::ENDC; }

        static std::string format(Type color, std::string_view text) {
            return std::format("{}{}{}", get_code(color), text, bcolors::ENDC);
        }

        static const char* get_code(Type color) {
            switch (color) {
            case None: return "";
            case Fail: return bcolors::FAIL;
            case OkGreen: return bcolors::OKGREEN;
            case Info: return bcolors::INFO;
            case OkCyan: return bcolors::OKCYAN;
            case Header: return bcolors::HEADER;
            case Warning: return bcolors::WARNING;
            case Bold: return bcolors::BOLD;
            case Underline: return bcolors::UNDERLINE;
            case Blue: return bcolors::BLUE;
            case Magenta: return bcolors::MAGENTA;
            default: return bcolors::ENDC;
            }
        }

    private:
        void apply_color(Type color) {
            m_os << get_code(color);
        }

    private:
        std::ostream& m_os;
    };
}

#endif
