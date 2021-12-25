#include <fmt/core.h>
#include <generate_interface.h>

constexpr auto interface_template =
    R"(#include <memory>

class {0} {{

    struct concept_ {{
{1}
        virtual ~concept_() = default;
    }};

    template <typename T> struct model_ : concept_ {{
        model_(const T& t) : value_(t) {{}}

{2}

        T value_;
    }};

public:
    template <typename T> {0}(const T &t) {{
        value_ = std::make_unique<model_<T>>(t);
    }}

{3}

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

std::string generate_concept_methods(const std::vector<Duck::Method> &methods) {
    std::string concept_methods;
    for (auto method = begin(methods); method != end(methods);) {
        auto arguments = generate_arguments(method->parameters);
        concept_methods +=
            fmt::format("        virtual {} {}_({}){} = 0;", method->type.name,
                        method->name, arguments, method->constant ? " const" : "");
        if (++method == end(methods))
            break;

        concept_methods += "\n";
    }
    return concept_methods;
}

std::string generate_model_methods(const std::vector<Duck::Method> &methods) {
    std::string model_methods;
    for (auto method = begin(methods); method != end(methods);) {
        auto arguments = generate_arguments(method->parameters);
        auto parameters = generate_parameters(method->parameters);
        auto ret = method->type.name == "void" ? "" : "return ";
        if (method->type.free_function) {
            model_methods += fmt::format(
                "        {0} {1}_({2}){6} override {{ {4}{1}(value_{5}{3}); }}",
                method->type.name, method->name, arguments, parameters, ret,
                !parameters.empty() ? ", " : "", method->constant ? " const" : "");
        } else {
            model_methods += fmt::format(
                "        {0} {1}_({2}){5} override {{ {4}value_.{1}({3}); }}",
                method->type.name, method->name, arguments, parameters, ret, method->constant ? " const" : "");
        }
        if (++method == end(methods))
            break;

        model_methods += "\n";
    }
    return model_methods;
}

std::string
generate_interface_methods(const std::vector<Duck::Method> &methods) {
    std::string interface_methods;
    for (auto method = begin(methods); method != end(methods);) {
        auto arguments = generate_arguments(method->parameters);
        auto parameters = generate_parameters(method->parameters);
        auto ret = method->type.name == "void" ? "" : "return ";
        interface_methods += fmt::format(
            "    {0} {1}({2}){5} {{ {4}value_->{1}_({3}); }}", method->type.name,
            method->name, arguments, parameters, ret, method->constant ? " const" : "");
        if (++method == end(methods))
            break;

        interface_methods += "\n";
    }
    return interface_methods;
}

void generate_interface(const Duck::Interface &interface) {

    std::string concept_methods = generate_concept_methods(interface.methods);

    std::string model_methods = generate_model_methods(interface.methods);

    std::string interface_methods =
        generate_interface_methods(interface.methods);

    fmt::print(interface_template, interface.name, concept_methods,
               model_methods, interface_methods);
}
