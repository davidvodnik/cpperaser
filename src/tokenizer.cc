#include "tokenizer.h"

namespace {

bool isspecial(char c) {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == '<' ||
           c == '>' || c == '[' || c == ']' || c == ',' || c == ':';
}

} // namespace

namespace Duck {

void Tokenizer::advance() {
    while (pos_ < s_.size() && std::isspace(s_[pos_])) {
        if (s_[pos_] == '\n') {
            line_++;
            line_pos_ = pos_ + 1;
        }
        pos_++;
    }

    first_ = pos_;

    if (pos_ == s_.size()) {
        token_ = Token{""};
        pos_++;
        return;
    }

    if (isspecial(s_[pos_])) {
        token_ = {std::string_view{s_.c_str() + first_, ++pos_ - first_}};
        return;
    }

    while (pos_ < s_.size() && !std::isspace(s_[pos_]) &&
           !isspecial(s_[pos_])) {
        token_ = {std::string_view{s_.c_str() + first_, ++pos_ - first_}};
    }
}

} // namespace Duck
