#pragma once
#include <string>

namespace CppEraser {

struct Line {
    std::string_view contents;
    size_t number;
    size_t position;
};

} // namespace CppEraser
