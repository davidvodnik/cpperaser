#pragma once
#include <string>
#include <tokenizer.h>
#include <vector>

namespace CppEraser {

struct Type {
    std::string name;
};

struct Parameter {
    Type type;
    std::string name;
};

struct Method {
    Method(Type type, std::string_view name, std::vector<Parameter> parameters,
           bool free_function = false, bool constant = false)
        : type(std::move(type)), name(name), parameters(std::move(parameters)),
          free_function(free_function), constant(constant) {}
    Type type;
    std::string name;
    std::vector<Parameter> parameters;
    bool free_function;
    bool constant;
};

class Interface {
public:
    Interface(std::string_view name, std::vector<Method> methods)
        : name(name), methods(std::move(methods)) {}

    std::string name;
    std::vector<Method> methods;
};

} // namespace CppEraser
