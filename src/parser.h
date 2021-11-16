#pragma once
#include "tokenizer.h"
#include <utility>
#include <variant>
#include <vector>

namespace Duck {

struct UnexpectedToken {
    std::string expected_;
    std::string_view token;
    Line line;
};

struct EndOfStream {
    std::string_view token;
    Line line;
};

struct InvalidName {
    std::string_view token;
    Line line;
};

class Error {
public:
    template <typename T> Error(T error) : error_(error) {}

    template <typename T> [[nodiscard]] bool is() const {
        return std::holds_alternative<T>(error_);
    }
    template <typename T> T as() const { return std::get<T>(error_); }

private:
    std::variant<UnexpectedToken, EndOfStream, InvalidName> error_;
};

template <typename T> class Result {
public:
    Result(T value) : value_(value) {}
    Result(Error error) : value_(error) {}

    bool valid() { return std::holds_alternative<T>(value_); }
    T value() { return std::get<T>(value_); }
    Error error() { return std::get<Error>(value_); }

private:
    std::variant<T, Error> value_;
};

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

Result<int> parse_expected(Tokenizer &t, const std::string &e);
Result<std::string_view> parse_name(Tokenizer &t);
Result<int> parse_nested_types(Tokenizer &t);
Result<Type> parse_type(Tokenizer &t);
Result<Parameter> parse_parameter(Tokenizer &t);
Result<std::vector<Parameter>> parse_parameter_list(Tokenizer &t);
Result<Method> parse_method(Tokenizer &t);
Result<std::vector<Method>> parse_methods(Tokenizer &t);
Result<Interface> parse_interface(Tokenizer &t);

} // namespace Duck
