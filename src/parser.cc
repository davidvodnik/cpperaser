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
    return Method{type.token_, name.token_};
}

} // namespace Duck
