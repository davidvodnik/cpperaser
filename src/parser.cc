#include "parser.h"
#include <cctype>

namespace Duck {

Result<std::string_view> parse_name(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (!std::isalpha(t.token()[0])) {
        return Error{InvalidName{t.token(), t.line()}};
    }
    return t.token();
}

Result<Method> parse_method(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    auto type = t.token();
    t.next();
    std::string_view name;
    if (auto n = parse_name(t); !n.valid()) {
        return n.error();
    } else {
        name = n.value();
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != "(") {
        return Error{UnexpectedToken{"(", t.token(), t.line()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != ")") {
        return Error{UnexpectedToken{")", t.token(), t.line()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != ";") {
        return Error{UnexpectedToken{";", t.token(), t.line()}};
    }
    t.next();
    return Method{type, name};
}

Result<Interface> parse_interface(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != "struct") {
        return Error{UnexpectedToken{"struct", t.token(), t.line()}};
    }
    t.next();
    std::string_view name;
    if (auto n = parse_name(t); !n.valid()) {
        return n.error();
    } else {
        name = n.value();
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != "{") {
        return Error{UnexpectedToken{"{", t.token(), t.line()}};
    }
    t.next();
    std ::vector<Method> methods;
    while (t.token() != "}" && t.valid()) {
        auto m = parse_method(t);
        if (!m.valid()) {
            return m.error();
        }
        methods.push_back(m.value());
    }
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != "}") {
        return Error{UnexpectedToken{"}", t.token(), t.line()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != ";") {
        return Error{UnexpectedToken{";", t.token(), t.line()}};
    }
    t.next();
    return Interface{name, methods};
}

} // namespace Duck
