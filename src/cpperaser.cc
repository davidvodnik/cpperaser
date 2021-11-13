#include "parser.h"
#include <fmt/core.h>

void print_interface(const Duck::Interface &interface) {
    fmt::print("class {} {{\n"
               "public:\n",
               interface.name_);

    for (const auto &m : interface.methods_) {
        fmt::print("    {} {}();\n", m.type_, m.name_);
    }

    fmt::print("    template<typename T>\n"
               "    {}(T& t) {{\n"
               "        val_ = reinterpret_cast<uintptr_t>(&t);\n",
               interface.name_);

    for (const auto &m : interface.methods_) {
        fmt::print("        {1}_ = [](uintptr_t t) -> {0} {{ "
                   "reinterpret_cast<T*>(t)->{1}(); }};\n",
                   m.type_, m.name_);
    }

    fmt::print("    }}\n"
               "private:\n");

    for (const auto &m : interface.methods_) {
        fmt::print("    {} (*{}_)(uintptr_t t);\n", m.type_, m.name_);
    }

    fmt::print("    uintptr_t val_;\n"
               "}};");
}

void print_error(const Duck::Error &error) {
    if (error.is<Duck::UnexpectedToken>()) {
        auto e = error.as<Duck::UnexpectedToken>();
        fmt::print("Unexpected token, expected '{}', got '{}', line {} at {}\n",
                   e.expected_, e.token_.token_, e.token_.line(),
                   e.token_.position());
    }
    if (error.is<Duck::EndOfStream>()) {
        auto e = error.as<Duck::EndOfStream>();
        fmt::print("Unexpected end of stream, line {} at {}\n", e.token_.line(),
                   e.token_.position());
    }
}

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    Duck::Tokenizer t(argv[1]);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        print_error(interface.error());
        return -1;
    }

    print_interface(interface.value());
}
