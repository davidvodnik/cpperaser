#pragma once
#include <string>

namespace Duck {

struct Line {
    std::string_view contents;
    size_t number;
    size_t position;
};

class Tokenizer {
public:
    Tokenizer(std::string s) : s_(std::move(s)) { advance(); }

    bool valid() { return pos_ <= s_.size(); }

    bool next() {
        advance();
        return valid();
    }

    std::string_view &token() { return token_; }

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

    size_t pos_{};

    std::string_view token_;

    size_t first_{};
    size_t line_{};
    size_t line_pos_{};
};

} // namespace Duck
