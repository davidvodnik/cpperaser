#include "tokenizer.h"
#include <catch.hpp>

using namespace Duck;

TEST_CASE("Read whitespace") { REQUIRE(Tokenizer(" \n\t").valid() == false); }

TEST_CASE("Read special tokens") {
    REQUIRE(Tokenizer("  { ").token() == "{");
    REQUIRE(Tokenizer("  } ").token() == "}");
    REQUIRE(Tokenizer("  ( ").token() == "(");
    REQUIRE(Tokenizer("  ) ").token() == ")");
    REQUIRE(Tokenizer("  [ ").token() == "[");
    REQUIRE(Tokenizer("  ] ").token() == "]");
    REQUIRE(Tokenizer("  < ").token() == "<");
    REQUIRE(Tokenizer("  > ").token() == ">");
    REQUIRE(Tokenizer("  , ").token() == ",");
    REQUIRE(Tokenizer("  : ").token() == ":");
    REQUIRE(Tokenizer("  ; ").token() == ";");
}

TEST_CASE("Read {} brackets") {
    auto &&str = GENERATE(" {  } ", "{}");
    Tokenizer t(str);
    REQUIRE(t.token() == "{");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "}");
    REQUIRE(t.next() == false);
}

TEST_CASE("Read () brackets") {
    auto &&str = GENERATE(" (  ) ", "()");
    Tokenizer t(str);
    REQUIRE(t.token() == "(");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ")");
    REQUIRE(t.next() == false);
}

TEST_CASE("Read <> brackets") {
    auto &&str = GENERATE(" <  > ", "<>");
    Tokenizer t(str);
    REQUIRE(t.token() == "<");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ">");
    REQUIRE(t.next() == false);
}

TEST_CASE("Read [] brackets") {
    auto &&str = GENERATE(" [  ] ", "[]");
    Tokenizer t(str);
    REQUIRE(t.token() == "[");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "]");
    REQUIRE(t.next() == false);
}

TEST_CASE("Read string tokens") {
    REQUIRE(Tokenizer("  \"str\"   ").token() == "\"str\"");
    REQUIRE(Tokenizer("  str   ").token() == "str");
    REQUIRE(Tokenizer("  1  ").token() == "1");
    REQUIRE(Tokenizer("  42  ").token() == "42");
    REQUIRE(Tokenizer("  3.14  ").token() == "3.14");
    REQUIRE(Tokenizer("  3.14abc  ").token() == "3.14abc");
}

TEST_CASE("Read json") {
    auto &&str =
        GENERATE(" { \"field\" : \"value\" , \"list\" : [ 1, 42, 3.14 ] } ",
                 "{\"field\":\"value\",\"list\":[1,42,3.14]}");
    Tokenizer t(str);
    REQUIRE(t.token() == "{");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "\"field\"");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ":");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "\"value\"");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ",");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "\"list\"");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ":");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "[");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "1");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ",");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "42");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ",");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "3.14");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "]");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "}");
    REQUIRE(t.next() == false);
}

TEST_CASE("Read struct") {
    auto &&str =
        GENERATE("struct test {\n\tvoid x();\n};", "struct test{void x();};");
    Tokenizer t(str);
    REQUIRE(t.token() == "struct");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "test");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "{");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "void");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "x");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "(");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ")");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ";");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "}");
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ";");
    REQUIRE(t.next() == false);
}

TEST_CASE("Line numbers and token positions") {
    Tokenizer t("struct test {\n    void x();\n};");
    REQUIRE(t.token() == "struct");
    REQUIRE(t.line().number == 0);
    REQUIRE(t.line().position == 0);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "test");
    REQUIRE(t.line().number == 0);
    REQUIRE(t.line().position == 7);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "{");
    REQUIRE(t.line().number == 0);
    REQUIRE(t.line().position == 12);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "void");
    REQUIRE(t.line().number == 1);
    REQUIRE(t.line().position == 4);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "x");
    REQUIRE(t.line().number == 1);
    REQUIRE(t.line().position == 9);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "(");
    REQUIRE(t.line().number == 1);
    REQUIRE(t.line().position == 10);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ")");
    REQUIRE(t.line().number == 1);
    REQUIRE(t.line().position == 11);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ";");
    REQUIRE(t.line().number == 1);
    REQUIRE(t.line().position == 12);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == "}");
    REQUIRE(t.line().number == 2);
    REQUIRE(t.line().position == 0);
    REQUIRE(t.next() == true);
    REQUIRE(t.token() == ";");
    REQUIRE(t.line().number == 2);
    REQUIRE(t.line().position == 1);
    REQUIRE(t.next() == false);
}

TEST_CASE("Line number at end of stream should point to end of stream") {
    Tokenizer t("struct");
    REQUIRE(t.next() == false);
    REQUIRE(t.line().position == 6);
}
