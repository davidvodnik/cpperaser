#include <fmt/core.h>
#include <print_error.h>

std::string print_line(CppEraser::Line line) {
    std::string r;
    r += fmt::format("{: >{}} | {}\n", line.number + 1, 5, line.contents);
    r += fmt::format("      | {: >{}}\n", "^", line.position + 1);
    return r;
}

std::string print_error(const CppEraser::Error &error) {
    std::string r;

    if (error.is<CppEraser::UnexpectedToken>()) {
        auto e = error.as<CppEraser::UnexpectedToken>();
        r += fmt::format(":{}:{}: error: expected '{}'\n", e.line.number + 1,
                         e.line.position + 1, e.expected_, e.token);
        r += print_line(e.line);
    }
    if (error.is<CppEraser::EndOfStream>()) {
        auto e = error.as<CppEraser::EndOfStream>();
        r += fmt::format(":{}:{}: error: end of stream\n", e.line.number + 1,
                         e.line.position + 1);
        r += print_line(e.line);
    }
    if (error.is<CppEraser::InvalidName>()) {
        auto e = error.as<CppEraser::InvalidName>();
        r += fmt::format(":{}:{}: error: invalid name '{}'\n",
                         e.line.number + 1, e.line.position + 1, e.token);
        r += print_line(e.line);
    }

    return r;
}
