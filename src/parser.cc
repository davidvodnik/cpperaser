#include "parser.h"
#include <cctype>

namespace Duck {

Result<Type> parse_type(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    auto type = t.token();
    t.next();
    return Type{std::string{type}};
}

Result<std::string_view> parse_name(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    auto name = t.token();
    if (!std::isalpha(name[0])) {
        return Error{InvalidName{t.token(), t.line()}};
    }
    t.next();
    return name;
}

Result<int> parse_expected(Tokenizer &t, const std::string &e) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }
    if (t.token() != e) {
        return Error{UnexpectedToken{e, t.token(), t.line()}};
    }
    t.next();
    return 0;
}

Result<Method> parse_method(Tokenizer &t) {
    Type type;
    if (auto n = parse_type(t); !n.valid()) {
        return n.error();
    } else {
        type = n.value();
    }
    std::string_view name;
    if (auto n = parse_name(t); !n.valid()) {
        return n.error();
    } else {
        name = n.value();
    }
    if (auto e = parse_expected(t, "("); !e.valid()) {
        return e.error();
    }
    if (auto e = parse_expected(t, ")"); !e.valid()) {
        return e.error();
    }
    if (auto e = parse_expected(t, ";"); !e.valid()) {
        return e.error();
    }
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
    if (auto e = parse_expected(t, "{"); !e.valid()) {
        return e.error();
    }
    std ::vector<Method> methods;
    while (t.token() != "}" && t.valid()) {
        auto m = parse_method(t);
        if (!m.valid()) {
            return m.error();
        }
        methods.push_back(m.value());
    }
    if (auto e = parse_expected(t, "}"); !e.valid()) {
        return e.error();
    }
    if (auto e = parse_expected(t, ";"); !e.valid()) {
        return e.error();
    }
    return Interface{name, methods};
}

} // namespace Duck
