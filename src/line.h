#pragma once
#include <string>

namespace Duck {

struct Line {
    std::string_view contents;
    size_t number;
    size_t position;
};

} // namespace Duck
