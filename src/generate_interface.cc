#include <fmt/core.h>
#include <generate_interface.h>

constexpr auto interface_template =
R"(#include <memory>

class {0} {{

    struct concept_ {{
{1}
    }}

    template<typename T> struct model_ : concept_ {{
        model_(T t) : value_(t) {{}}
{2}
        T value_;
    }};

public:
{3}

    template <typename T> Drawable(const T &t) {{
        value_ = std::make_unique<model_<T>>(t);
    }}

private:
    std::unique_ptr<concept_> value_;
}};)";

void generate_interface(const Duck::Interface &interface) {

    std::string concept_methods;
    for (const auto &m : interface.methods) {
        concept_methods +=
            fmt::format("        virtual {} {}_();\n", m.type.name, m.name);
    }

    std::string model_methods;
    for (const auto &m : interface.methods) {
        auto ret = m.type.name == "void" ? "" : "return ";
        model_methods +=
            fmt::format("        {0} {1}_() override {{ {2}value_.{1}(); }}\n",
                        m.type.name, m.name, ret);
    }

    std::string interface_methods;
    for (const auto &m : interface.methods) {
        auto ret = m.type.name == "void" ? "" : "return ";
        interface_methods += fmt::format(
            "    {0} {1}() {{ {2}value_->{1}_(); }}\n", m.type.name, m.name, ret);
    }

    fmt::print(interface_template, interface.name, concept_methods,
               model_methods, interface_methods);
}
