#include "parser.h"
#include <cctype>

namespace Duck {

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

Result<int> parse_nested_types(Tokenizer &t) {
    if (t.token() == "<") {
        do {
            t.next();

            auto nested_type = parse_type(t);
            if (!nested_type.valid()) {
                return nested_type.error();
            }
        } while (t.token() == ",");

        if (auto e = parse_expected(t, ">"); !e.valid()) {
            return e.error();
        }
    }
    return 0;
}

Result<Type> parse_type(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token(), t.line()}};
    }

    auto name = parse_name(t);
    if (!name.valid()) {
        return name.error();
    }

    if (auto e = parse_nested_types(t); !e.valid()) {
        return e.error();
    }

    return Type{std::string{name.value()}};
}

Result<Method> parse_method(Tokenizer &t) {
    auto type = parse_type(t);
    if (!type.valid()) {
        return type.error();
    }

    auto name = parse_name(t);
    if (!name.valid()) {
        return name.error();
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

    return Method{type.value(), name.value()};
}

Result<std::vector<Method>> parse_methods(Tokenizer &t) {
    std ::vector<Method> methods;
    while (t.token() != "}" && t.valid()) {
        auto method = parse_method(t);
        if (!method.valid()) {
            return method.error();
        }
        methods.push_back(method.value());
    }
    return methods;
}

Result<Interface> parse_interface(Tokenizer &t) {
    if (auto e = parse_expected(t, "struct"); !e.valid()) {
        return e.error();
    }

    auto name = parse_name(t);
    if (!name.valid()) {
        return name.error();
    }

    if (auto e = parse_expected(t, "{"); !e.valid()) {
        return e.error();
    }

    auto methods = parse_methods(t);
    if (!methods.valid()) {
        return methods.error();
    }

    if (auto e = parse_expected(t, "}"); !e.valid()) {
        return e.error();
    }

    if (auto e = parse_expected(t, ";"); !e.valid()) {
        return e.error();
    }

    return Interface{name.value(), methods.value()};
}

} // namespace Duck
