#include <catch.hpp>
#include <parser.h>

using namespace Duck;

TEST_CASE("Parse type") {
    Tokenizer t("const int & x");
    auto type = parse_type(t);
    auto name = parse_name(t);
    REQUIRE(type.valid());
    REQUIRE(name.valid());
    REQUIRE(!t.valid());
}

TEST_CASE("Parse generic type") {
    Tokenizer t("optional<int> x");
    auto type = parse_type(t);
    auto name = parse_name(t);
    REQUIRE(type.valid());
    REQUIRE(name.valid());
    REQUIRE(!t.valid());
}

TEST_CASE("Parse type with namespace") {
    Tokenizer t("std::optional<int> x");
    auto type = parse_type(t);
    auto name = parse_name(t);
    REQUIRE(type.valid());
    REQUIRE(name.valid());
    REQUIRE(!t.valid());
}

TEST_CASE("Parse parameter list") {
    Tokenizer t("(int x)");
    auto m = parse_parameter_list(t);
    REQUIRE(m.valid());
    REQUIRE(m.value().size() == 1);
}

TEST_CASE("Parse method") {
    Tokenizer t("void fun(int x);");
    auto m = parse_method(t);
    REQUIRE(m.valid());
    REQUIRE(m.value().type.name == "void");
    REQUIRE(m.value().name == "fun");
    REQUIRE(m.value().parameters.size() == 1);
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
    REQUIRE(i.error().as<UnexpectedToken>().token == "xstruct");
}
