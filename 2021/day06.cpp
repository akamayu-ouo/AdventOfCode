#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <string>

using Input = std::vector<size_t>;


Input read(std::istream& is) {
	Input in;
	for(std::string num; std::getline(is, num, ','); )
		in.emplace_back(std::stoll(num));
	return in;
}

size_t after(const Input& in, unsigned days) {
	std::array<unsigned, 9> cnt{0U};
	for(auto i : in) cnt[i]++;
	for(auto i = 0U, zero = 0U, six = 6U; i < days; ++i){
		// the 6th after this day += the 0th before this day
		cnt[(++six)%9] += cnt[(zero++)%9];
	}
	return std::accumulate(cnt.cbegin(), cnt.cend(), 0ULL);
}

int main(void) {
	auto in{read(std::cin)};
	std::cout << "Answer 1:" << after(in,  80) << '\n';
	std::cout << "Answer 2:" << after(in, 256) << '\n';
}
