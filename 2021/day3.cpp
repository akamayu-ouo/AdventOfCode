#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<ranges>

auto at(size_t i){
        return [=](const auto& l){return l[i];};
}

size_t part1(const auto& v) {
        const size_t n = v.front().size();
        size_t gma = 0;
        size_t eps = 0;
        for(unsigned i = 0; i < n; ++i){
                auto n1 = std::ranges::count(v, '1', at(i));
                auto n0 = v.size() - n1;
                gma = (gma*2)+(n1>n0);
                eps = (eps*2)+(n1<n0);
        }
        return gma*eps;
}

template<typename cmp>
size_t part2_(auto&& v, size_t i = 0) {
        if(v.size() == 1) return std::stol(v.front(), nullptr, 2);
        auto r = std::ranges::partition(v, [](char c){return c=='0';}, at(i));
        auto n1 = r.size(), n0 = v.size() - n1;
        return part2_<cmp>(cmp{}(n0, n1)? r : decltype(r){v.begin(), r.begin()}, i+1);
}

template<typename cmp>
size_t part2__(auto a, auto b) {
        const size_t n = b - a;
        for(size_t i = 0; (i < n) && (b-a > 1); ++i){
                auto m = std::partition(a, b, [=](const auto& l){return l[i]=='0';});
                auto n1 = b - m, n0 = m - a;
                (cmp{}(n0,n1)? a: b) = m;
        }
        return std::stol(*a, nullptr, 2);
}

size_t part2(auto v) {
        auto oxy_rate = part2_<std::less_equal<>>(v);
        auto co2_rate = part2_<std::greater<>>(v);
        return co2_rate * oxy_rate;
}

int main(void) {
        std::vector<std::string> v;
        for(std::string s; std::cin >> s; v.push_back(s));
        std::cout << "Answer1: " << part1(v) << '\n';
        std::cout << "Answer2: " << part2(v) << '\n';
        return 0;
}
