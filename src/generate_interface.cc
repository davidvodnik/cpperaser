#include <fmt/core.h>
#include <generate_interface.h>

void generate_interface(const Duck::Interface &interface) {
    fmt::print("#include <memory>\n\n"
               "class {} {{\n"
               "    struct concept_ {{\n",
               interface.name);

    for (const auto &m : interface.methods) {
        fmt::print("        virtual {} {}_();\n", m.type.name, m.name);
    }

    fmt::print("    }};\n"
               "\n"
               "    template<typename T> struct model_ : concept_ {{\n"
               "        model_(T t) : value_(t) {{}}\n");

    for (const auto &m : interface.methods) {
        auto ret = m.type.name == "void" ? "" : "return ";
        fmt::print("        {} {}_() override {{ {}value_.draw(); }}\n",
                   m.type.name, m.name, ret);
    }

    fmt::print("        T value_;\n"
               "    }};\n"
               "\n"
               "public:\n");

    for (const auto &m : interface.methods) {
        auto ret = m.type.name == "void" ? "" : "return ";
        fmt::print("    {} {}() {{ {}value_->draw_(); }}\n", m.type.name,
                   m.name, ret);
    }

    fmt::print("\n"
               "    template <typename T> Drawable(const T &t) {{\n"
               "        value_ = std::make_unique<model_<T>>(t);\n"
               "    }}\n"
               "\n"
               "private:\n"
               "    std::unique_ptr<concept_> value_;\n"
               "}};\n");
}
