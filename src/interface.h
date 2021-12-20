#pragma once
#include <string>
#include <tokenizer.h>
#include <vector>

namespace Duck {

struct Type {
    bool free_function = false;
    std::string name;
};

struct Parameter {
    Type type;
    std::string name;
};

struct Method {
public:
    Method(Type type, std::string_view name, std::vector<Parameter> parameters,
           bool constant)
        : type(std::move(type)), name(name), parameters(std::move(parameters)),
          constant(constant) {}
    Type type;
    std::string name;
    std::vector<Parameter> parameters;
    bool constant;
};

class Interface {
public:
    Interface(std::string_view name, std::vector<Method> methods)
        : name(name), methods(std::move(methods)) {}

    std::string name;
    std::vector<Method> methods;
};

} // namespace Duck
