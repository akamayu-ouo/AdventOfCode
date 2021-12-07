#include <iostream> 
#include <vector>
#include <numeric>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace vw = rg::views;

/*
 *       1   2   3   4   5   6   7   8 
 * A =   a1  a2  a3 
 * B =       b1  b2  b3
 * C =           c1  c2  c3
 *
 *          A < B  <->  a1 < b3
 *          B < C  <->  b1 < c3
 */
template<size_t N>
size_t count_ascending_sliding_sum(const auto& v) {
        return std::transform_reduce(v.cbegin() + N, v.cend(), v.cbegin(), 0UL, std::plus<>{}, std::greater<>{});
        static_assert(N > 0);
        return rg::count_if(v | vw::sliding(N+1),
                            [](auto r){return r.front() < r.back();});
}

size_t part1(const auto& v) {
        return count_ascending_sliding_sum<1>(v);
}

size_t part2(const auto& v) {
        return count_ascending_sliding_sum<3>(v);
}

int main(void) {
        std::vector<int> v;
        for(int i = 0; std::cin >> i ; v.push_back(i));
        std::cout << "Answer1: " << part1(v) << '\n';
        std::cout << "Answer2: " << part2(v) << '\n';
        return 0;
}

