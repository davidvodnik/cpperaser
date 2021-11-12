#pragma once
#include "tokenizer.h"
#include <optional>

namespace Duck {

class Method {
public:
    Method(std::string_view type, std::string_view name)
        : type_(type), name_(name) {}
    std::string type_, name_;
};

std::optional<Method> parse_method(Tokenizer &t);

} // namespace Duck
