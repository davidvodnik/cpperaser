#include "fmt/format.h"
#include <generate_interface.h>
#include <parser.h>
#include <print_error.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

extern "C" {

EMSCRIPTEN_KEEPALIVE
int parse_interface(const char *input, char *output) {
    Duck::Tokenizer t(input);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        auto s = print_error(interface.error());
        strcpy(output, s.c_str());
        return 0;
    }

    auto s = generate_interface(interface.value());
    strcpy(output, s.c_str());

    return 1;
}
}

#else

#include <fmt/core.h>

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    Duck::Tokenizer t(argv[1]);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        fmt::print("{}", print_error(interface.error()));
        return -1;
    }

    fmt::print("{}", generate_interface(interface.value()));
}

#endif
