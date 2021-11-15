#pragma once
#include <string>

namespace Duck {

class Token {
public:
    Token() = default;
    Token(std::string_view token) : token_(token) {}

    bool operator==(const Token &token) const { return token_ == token.token_; }
    bool operator==(const std::string_view &token) const {
        return token_ == token;
    }
    bool operator!=(const std::string_view &token) const {
        return !(*this == token);
    }

    std::string_view token_;
};

struct Line {
    std::string_view contents_;
    size_t number;
    size_t position;
};

class Tokenizer {
public:
    Tokenizer(std::string s) : s_(std::move(s)), pos_(0), token_("") {
        advance();
    }

    bool valid() { return pos_ <= s_.size(); }

    bool next() {
        advance();
        return valid();
    }

    Token &token() { return token_; }

    Line line() {
        size_t pos = pos_;
        while (pos < s_.size() && s_[pos] != '\n')
            pos++;
        return {std::string_view(s_).substr(line_pos_, pos - line_pos_), line_,
                first_ - line_pos_};
    }

private:
    void advance();

    const std::string s_;

    size_t pos_;

    Token token_;

    size_t first_{};
    size_t line_{};
    size_t line_pos_{};
};

} // namespace Duck
