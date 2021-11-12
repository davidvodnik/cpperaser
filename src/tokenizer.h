#include <string>

namespace {

bool isspecial(char c) {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == '<' ||
           c == '>' || c == '[' || c == ']' || c == ',' || c == ':';
}

} // namespace

namespace Duck {

class Token {
public:
    Token(std::string_view token, size_t line, size_t position) : token_(token), line_(line), position_(position) {}

    bool operator==(const std::string_view &token) const {
        return token_ == token;
    }
    [[nodiscard]] size_t line() const { return line_; }
    [[nodiscard]] size_t position() const { return position_; }

    size_t line_;
    size_t position_;
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

private:
    void advance() {
        while (pos_ < s_.size() && std::isspace(s_[pos_])) {
            if (s_[pos_] == '\n') {
                line_++;
                line_pos_ = pos_ + 1;
            }
            pos_++;
        }
        if (pos_ == s_.size()) {
            pos_++;
            return;
        }

        size_t first = pos_;

        if (isspecial(s_[pos_])) {
            token_ = {std::string_view{s_.c_str() + first, ++pos_ - first}, line_, first - line_pos_};
            return;
        }

        while (pos_ < s_.size() && !std::isspace(s_[pos_]) &&
               !isspecial(s_[pos_])) {
            token_ = {std::string_view{s_.c_str() + first, ++pos_ - first}, line_, first - line_pos_};
        }
    }

    const std::string s_;

    size_t pos_;
    Token token_;

    size_t line_{};
    size_t line_pos_{};
};

} // namespace Duck
