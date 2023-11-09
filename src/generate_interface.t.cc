#include <catch.hpp>
#include <generate_interface.h>

TEST_CASE("Parse interface") {
    CppEraser::Interface interface {
        "Drawable", {
            {
                {"int"}, "draw", {}
            }
        }
    };
    auto generated = generate_interface(interface, false);
    REQUIRE(generated == R"(#include <memory>

class Drawable {

    struct concept_ {
        virtual ~concept_() = default;
        virtual int draw_() = 0;
    };

    template <typename T> struct model_ : concept_ {
        model_(const T& t) : value_(t) {}
        int draw_() override { return value_.draw(); }

        T value_;
    };

public:
    template <typename T> Drawable(const T &t) {
        value_ = std::make_unique<model_<T>>(t);
    }

    int draw() { return value_->draw_(); }

private:
    std::unique_ptr<concept_> value_;
};
)");
}

TEST_CASE("Parse interface - copyable") {
    CppEraser::Interface interface {
        "Drawable", {
            {
                {"int"}, "draw", {}
            }
        }
    };
    auto generated = CppEraser::generate_interface(interface, true);
    REQUIRE(generated == R"(#include <memory>

class Drawable {

    struct concept_ {
        virtual ~concept_() = default;
        virtual std::unique_ptr<concept_> copy_() const = 0;
        virtual int draw_() = 0;
    };

    template <typename T> struct model_ : concept_ {
        model_(const T& t) : value_(t) {}
        std::unique_ptr<concept_> copy_() const override {
            return std::make_unique<model_>(value_);
        }
        int draw_() override { return value_.draw(); }

        T value_;
    };

public:
    template <typename T> Drawable(const T &t) {
        value_ = std::make_unique<model_<T>>(t);
    }
    Drawable(const Drawable& other) : value_(other.value_->copy_()) {}

    int draw() { return value_->draw_(); }

private:
    std::unique_ptr<concept_> value_;
};
)");
}
