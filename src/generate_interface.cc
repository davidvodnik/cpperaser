#include <fmt/core.h>
#include <generate_interface.h>
#include <numeric>

namespace CppEraser {

std::string
generate_arguments(const std::vector<CppEraser::Parameter> &parameters) {
    return std::accumulate(
        begin(parameters), end(parameters), std::string(), [](auto s, auto &p) {
            return (s.empty() ? "" : (s + ", ")) + p.type.name + " " + p.name;
        });
}

std::string
generate_parameters(const std::vector<CppEraser::Parameter> &parameters) {
    return std::accumulate(
        begin(parameters), end(parameters), std::string(),
        [](auto s, auto &p) { return (s.empty() ? "" : (s + ", ")) + p.name; });
}

constexpr auto concept_template =
    R"(    struct concept_ {{
        virtual ~concept_() = default;{0}
    }};)";
constexpr auto concept_copy_template =
    "\n        virtual std::unique_ptr<concept_> copy_() const = 0;";
constexpr auto concept_method_template = "\n        virtual {} {}_({}){} = 0;";

std::string
generate_concept_methods(const std::vector<CppEraser::Method> &methods,
                         bool copyable) {
    return fmt::format(
        concept_template,
        std::accumulate(
            begin(methods), end(methods),
            std::string(copyable ? concept_copy_template : ""),
            [](auto s, auto &method) {
                return s + fmt::format(concept_method_template,
                                       method.type.name, method.name,
                                       generate_arguments(method.parameters),
                                       method.constant ? " const" : "");
            }));
}

constexpr auto model_template =
    R"(    template <typename T> struct model_ : concept_ {{
        model_(const T& t) : value_(t) {{}}{0}

        T value_;
    }};)";
constexpr auto model_copy_template =
    R"(
        std::unique_ptr<concept_> copy_() const override {
            return std::make_unique<model_>(value_);
        })";

std::string
generate_model_methods(const std::vector<CppEraser::Method> &methods,
                       bool copyable) {
    return fmt::format(
        model_template,
        std::accumulate(
            begin(methods), end(methods),
            std::string(copyable ? model_copy_template : ""),
            [](auto s, auto &method) {
                auto arguments = generate_arguments(method.parameters);
                auto parameters = generate_parameters(method.parameters);
                auto ret = method.type.name == "void" ? "" : "return ";
                if (method.free_function) {
                    return s +
                           fmt::format("\n        {0} {1}_({2}){6} override {{ "
                                       "{4}{1}(value_{5}{3}); }}",
                                       method.type.name, method.name, arguments,
                                       parameters, ret,
                                       !parameters.empty() ? ", " : "",
                                       method.constant ? " const" : "");
                } else {
                    return s +
                           fmt::format("\n        {0} {1}_({2}){5} override {{ "
                                       "{4}value_.{1}({3}); }}",
                                       method.type.name, method.name, arguments,
                                       parameters, ret,
                                       method.constant ? " const" : "");
                }
            }));
}

constexpr auto methods_template =
    R"(public:
    template <typename T> {0}(const T &t) {{
        value_ = std::make_unique<model_<T>>(t);
    }}
{1})";
constexpr auto methods_copy_template =
    R"(    {0}(const {0}& other) : value_(other.value_->copy_()) {{}}
)";

std::string generate_interface_methods(Interface interface, bool copyable) {
    return fmt::format(
        methods_template, interface.name,
        std::accumulate(
            begin(interface.methods), end(interface.methods),
            std::string(copyable
                            ? fmt::format(methods_copy_template, interface.name)
                            : ""),
            [](auto s, auto &method) {
                return s +
                       fmt::format(
                           "\n    {0} {1}({2}){5} {{ {4}value_->{1}_({3}); }}",
                           method.type.name, method.name,
                           generate_arguments(method.parameters),
                           generate_parameters(method.parameters),
                           method.type.name == "void" ? "" : "return ",
                           method.constant ? " const" : "");
            }));
}

constexpr auto interface_template =
    R"(#include <memory>

class {0} {{

{1}

{2}

{3}

private:
    std::unique_ptr<concept_> value_;
}};
)";

std::string generate_interface(const CppEraser::Interface &interface,
                               bool copyable) {
    return fmt::format(interface_template, interface.name,
                       generate_concept_methods(interface.methods, copyable),
                       generate_model_methods(interface.methods, copyable),
                       generate_interface_methods(interface, copyable));
}

} // namespace CppEraser
