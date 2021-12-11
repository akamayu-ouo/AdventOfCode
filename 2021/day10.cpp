#include <iostream>
#include <vector>
#include <array>
#include <range/v3/all.hpp>

struct Result {
        bool corrupted;
        size_t val;
};

auto idx(char c) {
        switch (c) {
                case '(': return 1; case ')': return -1; 
                case '[': return 2; case ']': return -2; 
                case '{': return 3; case '}': return -3; 
                case '<': return 4; case '>': return -4; 
                default : return 0;
        }
}

Result calc_score(std::string_view sv) {
        static const std::array<size_t, 5> score{0UL, 3UL, 57UL, 1197UL, 25137UL};
        size_t N = 0; // Use number as stack (max depth 27)
        for(auto c : sv ) {
                if(auto m = idx(c); m > 0){
                       N = N * 5 + m;
                }else if(m < 0){
                       if(N % 5 != -m) return {true, score.at(-m)};
                       N /= 5;
                }
        }
        // Reverse the stack in base 5
        size_t M = 0;
        while(N) {
                M = M * 5 + (N % 5);
                N /= 5;
        }
        return {false, M};
}

size_t sum_values(auto r) {
        return ranges::accumulate(r, 0UL, ranges::plus{}, &Result::val);
}

size_t midean_value(auto r) {
        auto m = r.begin() + r.size()/2;
        ranges::nth_element(r, m, ranges::less{}, &Result::val);
        return m->val;
}

int main(void) {
        using namespace ranges;
        std::vector r = istream<std::string>(std::cin) 
                      | views::transform(calc_score) 
                      | to_vector;
        auto m = partition(r, identity{}, &Result::corrupted);
        auto corrupted  = subrange(r.begin(), m);
        auto incomplete = subrange(m, r.end());

        std::cout << "Answer 1: " << sum_values(corrupted)    << '\n';
        std::cout << "Answer 2: " << midean_value(incomplete) << '\n';
        return 0;
}
