#pragma once
#include <string>

namespace Duck {

class Token {
public:
    Token() = default;
    Token(std::string_view token, size_t line, size_t position)
        : token_(token), line_(line), position_(position) {}

    bool operator==(const Token &token) const {
        return token_ == token.token_ && line_ == token.line_ &&
               position_ == token.position_;
    }
    bool operator==(const std::string_view &token) const {
        return token_ == token;
    }
    bool operator!=(const std::string_view &token) const {
        return !(*this == token);
    }
    [[nodiscard]] size_t line() const { return line_; }
    [[nodiscard]] size_t position() const { return position_; }

    size_t line_{};
    size_t position_{};
    std::string_view token_;
};

class Tokenizer {
public:
    Tokenizer(std::string s) : s_(std::move(s)), pos_(0), token_("", 0, 0) {
        advance();
    }

    bool valid() { return pos_ <= s_.size(); }

    bool next() {
        advance();
        return valid();
    }

    Token &token() { return token_; }

    std::string_view line() {
        size_t pos = pos_;
        while (pos < s_.size() && s_[pos] != '\n')
            pos++;
        return std::string_view(s_).substr(line_pos_, pos - line_pos_);
    }

private:
    void advance();

    const std::string s_;

    size_t pos_;
    Token token_;

    size_t line_{};
    size_t line_pos_{};
};

} // namespace Duck
