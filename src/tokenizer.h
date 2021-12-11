#pragma once
#include <line.h>
#include <string>

namespace Duck {

class Tokenizer {
public:
    Tokenizer(std::string s) : s_(std::move(s)) { advance(); }

    bool valid() { return pos_ <= s_.size(); }

    bool next() {
        advance();
        return valid();
    }

    std::string_view &token() { return token_; }
    [[nodiscard]] size_t cursor_first() const { return first_; }
    [[nodiscard]] size_t cursor_last() const { return last_; }
    std::string_view from_cursor(size_t start, size_t end) {
        return std::string_view{s_.c_str() + start, end - start};
    }

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
    size_t last_{};

    size_t line_{};
    size_t line_pos_{};
};

} // namespace Duck
