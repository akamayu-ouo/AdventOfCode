#include <iostream>
#include <range/v3/all.hpp>
#include <vector>

namespace rg = ranges;
namespace rv = rg::views;
namespace ra = rg::actions;

auto match(auto n){
        return [=](size_t j){return 0 != (n&j);};
}

size_t part1(const auto& v, auto n) {
        size_t gma = 0, eps = 0;
        while(n > 0){
                auto n1 = rg::count_if(v, match(n));
                auto n0 = v.size() - n1;
                gma = (gma*2)+(n1>n0);
                eps = (eps*2)+(n1<n0);
                n >>= 1;
        }
        return gma*eps;
}

template<typename cmp>
size_t part2(const auto& v, auto n) {
        auto a = std::cbegin(v), b = std::cend(v);
        while((b-a)>1 && n>0){
                auto m = rg::find_if(a, b, match(n));
                auto n1 = b - m, n0 = m - a;
                (cmp{}(n0,n1)? a: b) = m;
                n >>= 1;
        }
        return *a;
}

size_t part2(auto v, auto n){
        auto oxy = part2<std::less_equal<>>(v, n);
        auto co2 = part2<std::greater<>>(v, n);
        return co2 * oxy;
}

int main(void) {
        const auto v = rg::getlines(std::cin)
                     | rv::transform([](auto&&s){return std::stoll(s, nullptr, 2);})
                     | rg::to_vector
                     | ra::sort;
        const size_t n = 0b1ULL << static_cast<size_t>(std::log2(rg::max(v)));
        std::cout << "Answer1: " << part1(v, n) << '\n';
        std::cout << "Answer2: " << part2(v, n) << '\n';
        return 0;
}
