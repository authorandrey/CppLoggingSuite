#ifndef CPP_LOGGING_SUITE_TABLE_H
#define CPP_LOGGING_SUITE_TABLE_H

#include "core/Logger.h"
#include <vector>
#include <string>
#include <algorithm>

namespace logger {

    class Table {
    public:
        Table() = default;

        explicit Table(std::vector<std::string> headers) : headers(std::move(headers)) {}

        void operator+=(std::vector<std::string> row) {
            rows.push_back(std::move(row));
            calculate_column_widths(rows.back());
        }

        template<typename... Args>
        void operator+=(Args... args) {
            std::vector<std::string> row{ std::forward<Args>(args)... };
            add_row(std::move(row));
        }

    private:
        void calculate_column_widths(const std::vector<std::string>& row) {
            if (column_widths.size() < row.size()) {
                column_widths.resize(row.size(), 0);
            }

            for (size_t i = 0; i < row.size(); ++i) {
                column_widths[i] = std::max(column_widths[i], row[i].size());
            }

            for (size_t i = 0; i < headers.size(); ++i) {
                if (i < column_widths.size()) {
                    column_widths[i] = std::max(column_widths[i], headers[i].size());
                }
            }
        }

        void print_row(const std::vector<std::string>& row) const {
            std::string line = "|";
            for (size_t i = 0; i < row.size(); ++i) {
                size_t width = i < column_widths.size() ? column_widths[i] : 0;
                line += std::format(" {:<{}} |", row[i], width);
            }
            std::cout << line << std::endl;
        }

        std::string create_border() const {
            if (column_widths.empty()) return "+";

            std::string border = "+";
            for (size_t width : column_widths) {
                border += std::string(width + 2, '-') + "+";
            }
            return border;
        }

        friend std::ostream& operator<<(std::ostream& os, const Table& table);

    private:
        std::vector<std::string> headers;
        std::vector<std::vector<std::string>> rows;
        std::vector<size_t> column_widths;
    };

    std::ostream& operator<<(std::ostream& os, const Table& table) {
        if (table.headers.empty() && table.rows.empty()) return os;

        std::string border = table.create_border();
        std::cout << border << std::endl;

        if (table.headers.empty() == false) {
            table.print_row(table.headers);
            std::cout << border << std::endl;
        }

        for (const auto& row : table.rows) {
            table.print_row(row);
        }

        if (table.rows.empty() == false) {
            std::cout << border << std::endl;
        }

        return os;
    }
}
#endif