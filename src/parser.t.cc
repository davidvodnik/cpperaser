#include "parser.h"
#include <catch.hpp>

using namespace Duck;

TEST_CASE("Parse method") {
    Tokenizer t("void fun();");
    auto m = parse_method(t);
    REQUIRE(m.has_value());
    REQUIRE(m.value().type_ == "void");
    REQUIRE(m.value().name_ == "fun");
}

TEST_CASE("Parse interface") {
    Tokenizer t("struct interface{void fun();};");
    auto i = parse_interface(t);
    REQUIRE(i.has_value());
    REQUIRE(i.value().name_ == "interface");
    REQUIRE(i.value().methods_.size() == 1);
}
