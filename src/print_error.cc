#include <fmt/core.h>
#include <print_error.h>

void print_line(Duck::Line line) {
    fmt::print("{: >{}} | {}\n", line.number + 1, 5, line.contents);
    fmt::print("      | {: >{}}\n", "^", line.position + 1);
}

void print_error(const Duck::Error &error) {
    if (error.is<Duck::UnexpectedToken>()) {
        auto e = error.as<Duck::UnexpectedToken>();
        fmt::print(":{}:{}: error: expected '{}'\n", e.line.number + 1,
                   e.line.position + 1, e.expected_, e.token);
        print_line(e.line);
    }
    if (error.is<Duck::EndOfStream>()) {
        auto e = error.as<Duck::EndOfStream>();
        fmt::print(":{}:{}: error: end of stream\n", e.line.number + 1,
                   e.line.position + 1);
        print_line(e.line);
    }
    if (error.is<Duck::InvalidName>()) {
        auto e = error.as<Duck::InvalidName>();
        fmt::print(":{}:{}: error: invalid name '{}'\n", e.line.number + 1,
                   e.line.position + 1, e.token);
        print_line(e.line);
    }
}
