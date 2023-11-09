#pragma once
#include <interface.h>
#include <string>

namespace CppEraser {

std::string generate_interface(const CppEraser::Interface &interface,
                               bool copyable);

} // namespace CppEraser
