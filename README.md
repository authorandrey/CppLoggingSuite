# C++ Logging Suite
**C++ Logging Suite** is a modern, feature-rich console logging library designed for C++23 and later. It provides colored output, structured logging components, and thread-safe operations with minimal overhead.

## Features

- **Colored Output**: Built-in color support with customizable color schemes
- **Multiple Log Levels**: Debug, Info, Success, Warning, Error, and Fatal
- **Table Support**: ASCII table formatting with automatic column sizing
- **Performance Timing**: High-resolution clock for measuring code execution
- **Progress Bars**: Customizable progress indicators for long-running operations
- **Structured Blocks**: Nestable and non-nestable logging blocks
- **Thread-Safe**: All operations are protected by mutexes
- **Timestamps**: Optional timestamp prefixes for log entries
- **Header-Only**: Easy integration - just include the headers

## Project Structure

```
logger/
├──logger.h         # Master include file
├── core/
│   ├── Color.h      # ANSI color codes and formatting
│   ├── Logger.h     # Main logger class with log levels
└── components/
    ├── Block.h      # Nestable and non-nestable blocks
    ├── Clock.h      # Performance timing utilities
    ├── ProgressBar.h # Progress bar implementation
    └── Table.h      # ASCII table formatter
```

## Quick Start

### Installation

Simply copy the header files to your project's include directory:

```bash
git clone https://github.com/authorandrey/CppLoggingSuite.git
```

### Basic Usage

```cpp
#include "logger/logger.h"

int main() {
    using logger::Logger;

    // Configure the logger
    Logger::Config& config = Logger::get_config();
    config.min_level = Logger::Level::Debug;
    config.show_timestamp = true;
    config.colors_enabled = true;

    // Log messages at different levels
    Logger::none("Will not be printed");
    Logger::debug("Debug message with number: {}", 42);
    Logger::info("Informational message");
    Logger::success("Operation completed successfully!");
    Logger::warning("This is a warning");
    Logger::error("An error occurred");
    Logger::fatal("A fatal error occured");

    return 0;
}
```

## Component Examples

### Table Logging

```cpp
#include "logger/components/Table.h"

int main() {
    logger::Table table({ "Name", "Age", "Occupation" });

    table += {"Alice", "28", "Engineer"};
    table += {"Bob", "35", "Designer"};
    table += {"Charlie", "42", "Manager"};

    std::cout << table << std::endl;

    return 0;
}
```

**Output:**
```
+---------+-----+------------+
| Name    | Age | Occupation |
+---------+-----+------------+
| Alice   | 28  | Engineer   |
| Bob     | 35  | Designer   |
| Charlie | 42  | Manager    |
+---------+-----+------------+
```

### Progress Bars

```cpp
#include "logger/components/ProgressBar.h"

int main() {
    logger::ProgressBar bar(100, "Processing");

    for (int i = 0; i <= 100; i += 10) {
        bar.update(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    bar.finish("Done!");

    return 0;
}
```

### Performance Timing

```cpp
#include "logger/components/Clock.h"

int main() {
    logger::Clock clock;

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    clock.elapsed("Task completed in", logger::Clock::MilliSeconds);
}
```

### Nested Blocks

```cpp
#include "logger/components/Block.h"

int main() {
    logger::Block outer("Outer Operation");
    logger::Block inner_1("Inner 1 Operation");
    logger::Block inner_2("Inner 2 Operation");
}
```

**Output**
```

-------------------------Outer Operation-------------------------

--------------------------Inner 1 Operation--------------------------

---------------------------Inner 2 Operation---------------------------
----------------------------------------------------------------------
--------------------------------------------------------------------
----------------------------------------------------------------
```

### Non-Nestable Blocks

```cpp
#include "logger/components/Block.h"

int main() {
    logger::NonNestableBlock block_1("1");
    logger::NonNestableBlock block_2("2");
    logger::NonNestableBlock block_3("3");
}
```

**Output**
```
-------------------------1-------------------------
--------------------------------------------------

-------------------------2-------------------------
--------------------------------------------------

-------------------------3-------------------------
--------------------------------------------------
```

## Configuration

The logger can be configured globally:

```cpp
// Get current configuration
auto& config = logger::Logger::get_config();

// Modify configuration
config.min_level = logger::Logger::Level::Info;  // Only show Info and above
config.show_timestamp = false;                   // Disable timestamps
config.colors_enabled = false;                   // Disable colors (for file output)

// Create config object to apply it afterwards
logger::Logger::Config config;
config.show_timestamp = true;

logger::Logger::set_config(config);
```

### Log Levels

| Level | Symbol | Color | Description |
|-------|--------|-------|-------------|
| None  | (none) | None  | Unclassified messages |
| Debug | D      | Blue  | Debug information |
| Info  | i      | Yellow | General information |
| Success | +    | Green | Successful operations |
| Warning | !    | Yellow | Warnings |
| Error | E      | Red   | Error conditions |
| Fatal | F      | Red   | Fatal errors |

## Advanced Usage

### Custom Titles

```cpp
// Using static method
logger::Logger::title("Application Startup");

// Using RAII wrapper
logger::Title title("Section Header");
// Title is automatically printed
```

### Direct Color Usage

```cpp
#include "core/Color.h"

// Apply color to stream
{
    logger::Color color(logger::Color::OkGreen);
    std::cout << "This text is green" << std::endl;
} // Color resets automatically

// Format string with color
std::string colored = logger::Color::format(
    logger::Color::Warning, 
    "Warning message"
);
```

## Requirements

- **C++ Compiler**: Supporting C++23 or later
- **Platform**: Windows, Linux, or macOS with ANSI terminal support