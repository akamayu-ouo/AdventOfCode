#include <iostream>
#include <range/v3/all.hpp>
#include <vector>
#include <array>

namespace rg = ranges;
namespace rv = ranges::views;

void run(size_t steps, const auto& pairs, auto& cfreq, auto& ccfreq) {
        for(auto _ : rv::iota(0ULL, steps)){
                auto nccfreq = ccfreq;
                for(auto [cc, ccf] : rv::enumerate(ccfreq)) {
                       if(!ccf) continue;
                       auto a = cc/26, b = cc%26;
                       auto c = pairs[cc];
                       nccfreq[a*26+b]-=ccf;
                       nccfreq[a*26+c]+=ccf;
                       nccfreq[c*26+b]+=ccf;
                       cfreq[c]       +=ccf;
                }
                ccfreq = nccfreq;
        }
}

int main(void) {
        auto c2s = [](char c)->short{return c - 'A';};
        const std::vector<short> s = rg::getlines(std::cin) 
                                   | rv::take(1) 
                                   | rv::join 
                                   | rv::transform(c2s)
                                   | rg::to_vector;

        std::array<short, 26*26> pairs{0};
        for(auto r : rg::getlines(std::cin)) {
                pairs[26*c2s(r[0])+c2s(r[1])] = c2s(r[6]);
        }

        // quantity of each character
        std::array<size_t,  26> cfreq{0};
        for(auto a : s) cfreq[a]++;

        // quantity of each pair of characters
        std::array<size_t, 26*26> ccfreq{0};
        for(auto [a, b] : rv::zip(s, s|rv::drop(1))) ccfreq[a*26+b]++;

        auto diff = [](const auto& r){
                auto [min, max] = rg::minmax(r 
                                | rv::filter([](auto s){return s > 0;}));
                return max - min;
        };

        run(10, pairs, cfreq, ccfreq);
        std::cout << "Answer 1: " << diff(cfreq) << '\n';

        run(30, pairs, cfreq, ccfreq);
        std::cout << "Answer 2: " << diff(cfreq) << '\n';
}
