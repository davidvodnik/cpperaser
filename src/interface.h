#pragma once
#include <string>
#include <tokenizer.h>
#include <vector>

namespace Duck {

struct Type {
    std::string name;
};

struct Parameter {
    Type type;
    std::string name;
};

struct Method {
public:
    Method(Type type, std::string_view name, std::vector<Parameter> parameters)
        : type(std::move(type)), name(name), parameters(std::move(parameters)) {
    }
    Type type;
    std::string name;
    std::vector<Parameter> parameters;
};

class Interface {
public:
    Interface(std::string_view name, std::vector<Method> methods)
        : name(name), methods(std::move(methods)) {}

    std::string name;
    std::vector<Method> methods;
};

} // namespace Duck