#include <iostream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <functional>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = rg::views;

using ll = long long;

auto read(std::istream& is) {
        return rg::getlines(is, ',') 
             | rv::transform([](const auto& s){return std::stoi(s);})
             | rg::to_vector;
}

size_t min_fuel(const auto& in, size_t(*dist)(ll,ll)) {
        auto calc_fuel = [&](ll i) {
                return rg::accumulate(in, 0ULL, rg::plus{}, std::bind_front(dist, i));
        };
        return rg::min(rv::iota(0LL, *rg::max_element(in)) | rv::transform(calc_fuel));
}

size_t linear(ll a, ll b) {
	return std::abs(a - b);
}

size_t quadradic(ll a, ll b) {
	auto d = linear(a, b);
	return d * (d+1) / 2;
}

int main(void) {
	auto in{read(std::cin)};
	std::cout << "Answer 1:" << min_fuel(in,    linear) << '\n';
	std::cout << "Answer 2:" << min_fuel(in, quadradic) << '\n';
}
