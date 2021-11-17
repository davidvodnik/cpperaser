#pragma once
#include <line.h>
#include <variant>

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

} // namespace Duck
