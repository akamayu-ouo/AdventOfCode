#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <ranges>

using Input = std::vector<int>;
using ll = long long;


Input read(std::istream& is) {
	Input in;
	for(std::string num; std::getline(is, num, ','); )
		in.emplace_back(std::stoll(num));
	return in;
}

size_t cal_fuel(ll i, const auto& in, size_t(*dist)(ll,ll)){
	return std::transform_reduce(
			in.cbegin(), in.cend(), 
			0LL, std::plus<>{}, 
			std::bind_front(dist, i));
}

size_t min_fuel(const auto& in, size_t(*dist)(ll,ll)){
	auto n = *std::ranges::max_element(in);
	size_t min = std::numeric_limits<size_t>::max();
	for(long long  i = 0; i < n; ++i){
		auto fuel = cal_fuel(i, in, dist);
		if(min < fuel) return min;
		min = fuel;
	}
	return min;
}

size_t linear(ll a, ll b){
	return std::abs(a - b);
}

size_t quadradic(ll a, ll b){
	auto d = linear(a, b);
	return (d * d + d) / 2;
}

int main(void) {
	auto in{read(std::cin)};
	std::cout << "Answer 1:" << min_fuel(in,    linear) << '\n';
	std::cout << "Answer 2:" << min_fuel(in, quadradic) << '\n';
}
