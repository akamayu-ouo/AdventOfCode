#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = ranges::views;

size_t count_path(size_t now, const auto& edges, auto& visit_cnt, size_t end, const auto& abort){
        if(abort(now, visit_cnt)) return 0;
        if(now == end) return 1;
        visit_cnt[now]++;
        size_t cnt = 0;
        for(auto nxt : edges[now]){
                cnt += count_path(nxt, edges, visit_cnt, end, abort);
        }
        visit_cnt[now]--;
        return cnt;
}

auto read() {
        using namespace std;
        vector<vector<size_t>> edges{};
        vector<bool> lower{};

        unordered_map<std::string, size_t> index;
        auto get_idx = [&](const auto& s) {
                if(!index.contains(s)) {
                        edges.emplace_back();
                        lower.push_back(std::islower(s.front()));
                        index[s] = lower.size()-1;
                }
                return index[s];
        };

        for(string line : rg::getlines(cin)){
                auto i = line.find('-');
                auto a = get_idx(line.substr(0,i));
                auto b = get_idx(line.substr(i+1));
                edges[a].push_back(b);
                edges[b].push_back(a);
        }

        return make_tuple(edges, lower,  index["start"],  index["end"]);
}

int main(void) {
        const auto [edges, islower, start, end] = read();

        auto part1 = [&](size_t now, const auto& v){
                return islower.at(now) && v.at(now) > 0 ;
        };

        auto part2 = [&](size_t now, const auto& v) {
                if(!islower.at(now)) return false;
                if(v.at(now) == 2) return true;
                if(v.at(now) == 1){
                        if(now == start ||
                           rg::any_of(rv::zip(islower, v), [](const auto& i) {
                                   return i.first && i.second == 2;
                           })){
                                return true;
                        }
                }
                return false;
        };

        std::vector<size_t> visit_cnt(islower.size(), 0);

        std::cout << "Answer 1: " << count_path(start, edges, visit_cnt, end, part1) << '\n';
        std::cout << "Answer 2: " << count_path(start, edges, visit_cnt, end, part2) << '\n';
        return 0;
}
