#include "parser.h"

namespace Duck {

Result<Method> parse_method(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token()}};
    }
    auto type = t.token();
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    auto name = t.token();
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != "(") {
        return Error{UnexpectedToken{"(", t.token()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != ")") {
        return Error{UnexpectedToken{")", t.token()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != ";") {
        return Error{UnexpectedToken{";", t.token()}};
    }
    t.next();
    return Method{type.token_, name.token_};
}

Result<Interface> parse_interface(Tokenizer &t) {
    if (!t.valid()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != "struct") {
        return Error{UnexpectedToken{"struct", t.token()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    auto name = t.token();
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != "{") {
        return Error{UnexpectedToken{"{", t.token()}};
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
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != "}") {
        return Error{UnexpectedToken{"}", t.token()}};
    }
    if (!t.next()) {
        return Error{EndOfStream{t.token()}};
    }
    if (t.token() != ";") {
        return Error{UnexpectedToken{";", t.token()}};
    }
    t.next();
    return Interface{name.token_, methods};
}

} // namespace Duck
