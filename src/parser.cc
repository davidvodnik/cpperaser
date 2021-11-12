#include "parser.h"

namespace Duck {

std::optional<Method> parse_method(Tokenizer &t) {
    if (!t.valid()) {
        return {};
    }
    auto type = t.token();
    if (!t.next()) {
        return {};
    }
    auto name = t.token();
    if (!t.next()) {
        return {};
    }
    if (t.token() != "(") {
        return {};
    }
    if (!t.next()) {
        return {};
    }
    if (t.token() != ")") {
        return {};
    }
    if (!t.next()) {
        return {};
    }
    if (t.token() != ";") {
        return {};
    }
    t.next();
    return Method{type.token_, name.token_};
}

std::optional<Interface> parse_interface(Tokenizer &t) {
    if (!t.valid()) {
        return {};
    }
    if (t.token() != "struct") {
        return {};
    }
    if (!t.next()) {
        return {};
    }
    auto name = t.token();
    if (!t.next()) {
        return {};
    }
    if (t.token() != "{") {
        return {};
    }
    t.next();
    std::vector<Method> methods;
    while (t.token() != "}" && t.valid()) {
        auto m = parse_method(t);
        methods.push_back(m.value());
    }
    if (!t.valid()) {
        return {};
    }
    if (t.token() != "}") {
        return {};
    }
    if (!t.next()) {
        return {};
    }
    if (t.token() != ";") {
        return {};
    }
    t.next();
    return Interface{name.token_, methods};
}

} // namespace Duck
