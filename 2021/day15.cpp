#include <iostream>
#include <range/v3/all.hpp>
#include <vector>

namespace rg = ranges;
namespace rv = ranges::views;

size_t minimal_risk(size_t m, size_t n, const auto& map) {
	const size_t N = m*n;
	std::vector<size_t> dist(N, -1);
	std::vector<size_t> Q{0};
	dist[0] = 0;

	auto getdist = [&](auto i){return dist[i];};
	auto add = [&](size_t i, size_t j, size_t u) {
		if(i >= m || j >= n) return ;
		auto v = i*m+j, alt = dist[u] + map(i,j);
		if(alt < dist[v]) {
			dist[v] = alt;
			Q.push_back(v);
			rg::push_heap(Q, rg::greater{}, getdist);
		}
	};

	for(auto u = Q.back(); u != N-1 && !Q.empty(); u = Q.back()) {
		auto i = u/m, j = u%m;
		Q.pop_back();
		add(i+1, j, u); add(i-1, j, u);
		add(i, j+1, u); add(i, j-1, u);
		rg::pop_heap(Q, rg::greater{}, getdist);
	}
	return dist[N-1];
}

int main(void) {
	const auto c2s = [](auto&& c)->size_t{return c-'0';};
	const auto map = rg::getlines(std::cin)
		       | rv::transform([&](auto&& l){return l | rv::transform(c2s) 
					                       | rg::to_vector ;})
		       | rg::to_vector;
	const auto m = map.size(), n = map.front().size();
	auto access = [&](size_t i, size_t j) {
		size_t diff = i/m + j/n;
		size_t risk = map[i%m][j%n] + diff - 1 ;
		return risk % 9 + 1;
	};
	std::cout << "Answer 1: " << minimal_risk(m  ,n  , access) << '\n';
	std::cout << "Answer 2: " << minimal_risk(m*5,n*5, access) << '\n';
	return 0;
}
