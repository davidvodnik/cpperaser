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
