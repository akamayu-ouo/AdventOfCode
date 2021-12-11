#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <numeric>
#include <ranges>
#include <unordered_map>

struct Entry {
	std::array<std::string,10> sig;
	std::array<std::string,4> out;
};

using In = std::vector<Entry>;

In read(std::istream& is) {
	In in;
	for(std::string line ; std::getline(is, line); ) {
		std::istringstream iss{line};
		Entry e{};
                for(auto& sig : e.sig)
                        iss >> sig;
                iss.ignore(2);
                for(auto& out : e.out)
                        iss >> out;
                std::ranges::nth_element(e.sig, e.sig.begin() + 2, std::less<>{}, 
				[](const auto& s){return s.length();});
                //std::ranges::sort(e.sig, std::less<>{}, 2
		//		[](const auto& s){return s.length();});
		in.push_back(e);
	}
	return in;
}

size_t part1(const auto& in) {
        size_t N = 0UL;
        for(const auto& e: in) {
                for(const auto& s: e.out){
                        auto len = s.length();
                        N += len == 2 || len == 3 || len == 4 || len == 7;
                }
        }
        return N;
}

auto decode(const Entry& ent) {
        static const std::unordered_map<size_t, size_t> freq2onehot{
                {  8, 1UL << 6},  // a
                {  9, 1UL << 5},  // b
                { 11, 1UL << 4},  // c
                { 10, 1UL << 3},  // d
                {  4, 1UL << 2},  // e
                { 12, 1UL << 1},  // f
                {  7, 1UL << 0},  // g
        };

        static const std::unordered_map<size_t, size_t> digit{
                        {0b1110111, 0}, {0b0010010, 1}, 
                        {0b1011101, 2}, {0b1011011, 3}, 
                        {0b0111010, 4}, {0b1101011, 5}, 
                        {0b1101111, 6}, {0b1010010, 7}, 
                        {0b1111111, 8}, {0b1111011, 9}, 
        };

        static const std::array<size_t, 10> coeff {
                1,1,4,1,1,1,1,1,1,1
        };

        std::array<size_t, 7> freq{0};
        for(auto i = 0UL; i < 10; ++i){
		for(const auto& c : ent.sig[i])
                        freq[c - 'a'] += coeff[i];
        }

        auto N = 0UL;
        for(auto out : ent.out) {
                size_t num = 0UL;
                for(auto ch : out) 
                        num |= freq2onehot.at(freq[ch - 'a']);
                N = N * 10 + digit.at(num);
        }
        return N;
}

// https://www.reddit.com/r/adventofcode/comments/rc5s3z/2021_day_8_part_2_a_simple_fast_and_deterministic/
auto reddit(const Entry& e) {
	static const std::unordered_map<int, int> score {
		{42, 0}, {17, 1}, {34, 2}, {39, 3}, {30, 4}, {37, 5}, {41, 6}, {25, 7}, {49, 8}, {45, 9}
	};
        std::array<size_t, 7> freq{0};
        for(auto i = 0UL; i < 10; ++i){
		for(const auto& c : e.sig[i])
			freq[c - 'a']++;
	}
        auto N = 0UL;
        for(auto out : e.out) {
                auto s = 0UL;
                for(auto ch : out) 
			s += freq.at(ch - 'a');
                N = N * 10 + score.at(s);
        }
        return N;
}

size_t part2(const auto& in) {
        return std::transform_reduce(in.cbegin(), in.cend(),
                                     0UL, std::plus<>{}, reddit);
}

int main(void) {
        auto in{read(std::cin)};
        std::cout << "Answer 1: " << part1(in) << '\n';
        std::cout << "Answer 2: " << part2(in) << '\n';
}
