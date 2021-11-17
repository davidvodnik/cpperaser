#pragma once
#include <interface.h>
#include <result.h>
#include <tokenizer.h>

namespace Duck {

Result<int> parse_expected(Tokenizer &t, const std::string &e);
Result<std::string_view> parse_name(Tokenizer &t);
Result<int> parse_nested_types(Tokenizer &t);
Result<Type> parse_type(Tokenizer &t);
Result<Parameter> parse_parameter(Tokenizer &t);
Result<std::vector<Parameter>> parse_parameter_list(Tokenizer &t);
Result<Method> parse_method(Tokenizer &t);
Result<std::vector<Method>> parse_methods(Tokenizer &t);
Result<Interface> parse_interface(Tokenizer &t);

} // namespace Duck
