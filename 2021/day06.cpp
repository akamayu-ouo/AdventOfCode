#include <iostream>
#include <vector>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = rg::views;

using Input = std::vector<size_t>;

Input read(std::istream& is) {
        return rg::getlines(is, ',') 
             | rv::transform([](const auto& s)->size_t{return std::stoll(s);})
             | rg::to_vector;
}

size_t after(const Input& in, int days) {
	std::array<unsigned, 9> cnt{0U};
	for(auto i : in) cnt[i]++;
	for(auto zero = 0U, six = 6U; days > 0; days--){
		// the 6th after this day += the 0th before this day
		cnt[(++six)%9] += cnt[(zero++)%9];
	}
	return rg::accumulate(cnt, 0ULL);
}

int main(void) {
	auto in{read(std::cin)};
	std::cout << "Answer 1:" << after(in,  80) << '\n';
	std::cout << "Answer 2:" << after(in, 256) << '\n';
}
