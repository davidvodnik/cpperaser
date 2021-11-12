#pragma once
#include "tokenizer.h"
#include <optional>
#include <utility>
#include <vector>

namespace Duck {

class Method {
public:
    Method(std::string_view type, std::string_view name)
        : type_(type), name_(name) {}
    std::string type_, name_;
};

class Interface {
public:
    Interface(std::string_view name, std::vector<Method> methods)
        : name_(name), methods_(std::move(methods)) {}

    std::string name_;
    std::vector<Method> methods_;
};

std::optional<Method> parse_method(Tokenizer &t);

std::optional<Interface> parse_interface(Tokenizer &t);

} // namespace Duck
