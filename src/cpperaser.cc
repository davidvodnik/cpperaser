#include "fmt/format.h"
#include <generate_interface.h>
#include <parser.h>
#include <print_error.h>

#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

namespace Binding {

std::string generate_interface(std::string input, bool copyable) {
    CppEraser::Tokenizer t(input);
    auto interface = CppEraser::parse_interface(t);

    if (!interface.valid()) {
        return print_error(interface.error());
    }

    return CppEraser::generate_interface(interface.value(), copyable);
}

} // namespace Binding

EMSCRIPTEN_BINDINGS(cpperaser) {
    emscripten::function("generate_interface", &Binding::generate_interface);
}

#else

#include <fmt/core.h>

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    CppEraser::Tokenizer t(argv[1]);
    auto interface = CppEraser::parse_interface(t);

    if (!interface.valid()) {
        fmt::print("{}", print_error(interface.error()));
        return -1;
    }

    fmt::print("{}", generate_interface(interface.value(), true));
}

#endif
