#include <variant>
#include <vector>
#include <memory>
#include <map>

template<typename T>
struct wrapper {
    wrapper(const T& t) {
        p = std::make_unique<T>(t);
    }

    wrapper(const wrapper& other) {
        p = std::make_unique<T>(*other.p.get());
    }

    wrapper& operator= (const wrapper& other) {
        p = std::make_unique<T>(*other.p.get());
        return *this;
    }
    ~wrapper() = default;

    std::unique_ptr<T> p;
    auto operator<=> (const wrapper& other) const {
        return p <=> other.p;
    }
};

struct var : public std::variant<
    int,
    double,
    wrapper<std::vector<var> >,
    wrapper<std::map<var, var> >
> {
    using base = std::variant<
        int,
        double,
        wrapper<std::vector<var> >,
        wrapper<std::map<var, var> >
    >;
    using base::base;

    template<typename T>
    var(const T& t) : base{ wrapper<T>{t} }{}
};

int main() {
    var v1{ int{1} };
    var v2{ double{2.0} };
    var v3{ std::vector<var>{v1, v2} };

    std::map<var, var> m{};
    m[v1] = v2;
    var v4(m);
}
