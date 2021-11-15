#pragma once
#include "tokenizer.h"
#include <utility>
#include <variant>
#include <vector>

namespace Duck {

class UnexpectedToken {
public:
    UnexpectedToken(std::string expected, Token token, Line line)
        : expected_(std::move(expected)), token_(token), line_(line) {}

    std::string expected_;
    Token token_;
    Line line_;
};

class EndOfStream {
public:
    EndOfStream(Token token, Line line) : token_(token), line_(line) {}

    Token token_;
    Line line_;
};

class InvalidName {
public:
    InvalidName(Token token, Line line) : token_(token), line_(line) {}

    Token token_;
    Line line_;
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

Result<Method> parse_method(Tokenizer &t);

Result<Interface> parse_interface(Tokenizer &t);

} // namespace Duck
