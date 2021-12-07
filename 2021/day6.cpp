#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <numeric>
#include <string>

using Input = std::vector<size_t>;


Input read(std::istream& is) {
	Input in;
	std::string num;
	while(std::getline(is, num, ','))
		in.emplace_back(std::stoll(num));
	return in;
}

size_t after(const Input& in, unsigned days) {
	size_t cnt[9]{0};
	unsigned zero = 0, reset = 6;
	for(auto i : in) cnt[i]++;
	for(unsigned i = 0; i < days; ++i){
		reset = (reset + 1) % 9; // the sixth after this day
		cnt[reset] += cnt[zero];
		zero = (zero + 1) % 9;   // the zeroth before this day
		std::cout << '\n' << '{';
		std::cout << (i+1) << ": ";
		for(unsigned k = 0, l = zero; k < 9; ++k){
			std::cout << cnt[l] << ' ';
			l = (l+1)%9;
		}
		std::cout << '}';
	}
	return std::accumulate(cnt, cnt+9, 0ULL);
}

size_t part1(const Input& in) {
	return after(in, 80);
}

size_t part2(const Input& in) {
	return after(in, 256);
}

int main(void) {
	auto in{read(std::cin)};
	std::cout << "Answer 1:" << part1(in) << '\n';
	std::cout << "Answer 2:" << part2(in) << '\n';
}
