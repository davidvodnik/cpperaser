#include "parser.h"
#include <catch.hpp>

using namespace Duck;

TEST_CASE("Parse method") {
    Tokenizer t("void fun();");
    auto m = parse_method(t);
    REQUIRE(m.valid());
    REQUIRE(m.value().type.name == "void");
    REQUIRE(m.value().name == "fun");
}

TEST_CASE("Parse interface") {
    Tokenizer t("struct interface{void fun();};");
    auto i = parse_interface(t);
    REQUIRE(i.valid());
    REQUIRE(i.value().name == "interface");
    REQUIRE(i.value().methods.size() == 1);
}

TEST_CASE("Parse unexpected token") {
    Tokenizer t("xstruct interface{void fun();}");
    auto i = parse_interface(t);
    REQUIRE(!i.valid());
    REQUIRE(i.error().is<UnexpectedToken>());
    // REQUIRE(i.error().as<UnexpectedToken>().token_ == Token("xstruct", 0,
    // 0));
}
