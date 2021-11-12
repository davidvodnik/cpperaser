#include <string>

namespace {

bool isspecial(char c) {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == '<' ||
           c == '>' || c == '[' || c == ']' || c == ',' || c == ':';
}

} // namespace

namespace Duck {

class Tokenizer {
public:
    Tokenizer(std::string s) : s_(std::move(s)), pos_(0) { advance(); }

    bool valid() { return pos_ <= s_.size(); }

    bool next() {
        advance();
        return valid();
    }

    std::string_view token() { return token_; }

private:
    void advance() {
        while (pos_ < s_.size() && std::isspace(s_[pos_])) {
            pos_++;
        }
        if (pos_ == s_.size()) {
            pos_++;
            return;
        }

        size_t first = pos_;

        if (isspecial(s_[pos_])) {
            token_ = std::string_view{s_.c_str() + first, ++pos_ - first};
            return;
        }

        while (pos_ < s_.size() && !std::isspace(s_[pos_]) &&
               !isspecial(s_[pos_])) {
            token_ = std::string_view{s_.c_str() + first, ++pos_ - first};
        }
    }

    const std::string s_;

    size_t pos_;
    std::string_view token_;
};

} // namespace Duck
