#include <fmt/core.h>
#include <generate_interface.h>

constexpr auto interface_template =
    R"(#include <memory>

class {0} {{

    struct concept_ {{
{1}
    }};

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

}};
)";

std::string generate_arguments(const std::vector<Duck::Parameter> &parameters) {
    std::string argument_list;
    for (auto parameter = begin(parameters); parameter != end(parameters);) {
        argument_list +=
            fmt::format("{} {}", parameter->type.name, parameter->name);
        if (++parameter == end(parameters))
            break;

        argument_list += ", ";
    }
    return argument_list;
}

std::string
generate_parameters(const std::vector<Duck::Parameter> &parameters) {
    std::string parameter_list;
    for (auto parameter = begin(parameters); parameter != end(parameters);) {
        parameter_list += fmt::format("{}", parameter->name);
        if (++parameter == end(parameters))
            break;

        parameter_list += ", ";
    }
    return parameter_list;
}

void generate_interface(const Duck::Interface &interface) {

    std::string concept_methods;
    for (auto method = begin(interface.methods);
         method != end(interface.methods);) {
        auto arguments = generate_arguments(method->parameters);
        concept_methods +=
            fmt::format("        virtual {} {}_({}) = 0;", method->type.name,
                        method->name, arguments);
        if (++method == end(interface.methods))
            break;

        concept_methods += "\n";
    }

    std::string model_methods;
    for (auto method = begin(interface.methods);
         method != end(interface.methods);) {
        auto arguments = generate_arguments(method->parameters);
        auto parameters = generate_parameters(method->parameters);
        auto ret = method->type.name == "void" ? "" : "return ";
        model_methods += fmt::format(
            "        {0} {1}_({2}) override {{ {4}value_.{1}({3}); }}",
            method->type.name, method->name, arguments, parameters, ret);
        if (++method == end(interface.methods))
            break;

        model_methods += "\n";
    }

    std::string interface_methods;
    for (auto method = begin(interface.methods);
         method != end(interface.methods);) {
        auto arguments = generate_arguments(method->parameters);
        auto parameters = generate_parameters(method->parameters);
        auto ret = method->type.name == "void" ? "" : "return ";
        interface_methods += fmt::format(
            "    {0} {1}({2}) {{ {4}value_->{1}_({3}); }}", method->type.name,
            method->name, arguments, parameters, ret);
        if (++method == end(interface.methods))
            break;

        interface_methods += "\n";
    }

    fmt::print(interface_template, interface.name, concept_methods,
               model_methods, interface_methods);
}
