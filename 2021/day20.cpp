#include <iostream>
#include <vector>
#include <string>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = ranges::views;

auto surrounding(int i, int j, int d=1) {
	return rv::cartesian_product(rv::iota(i-d,i+d+1), rv::iota(j-d,j+d+1));
}

auto expand(int m, int n, int d=1) {
	return rv::cartesian_product(rv::iota(-d,m+d), rv::iota(-d,n+d));
}

void enhance(auto& world, auto far, const auto& IEA, size_t steps) {
	for(auto&& t : rv::iota(0UL, steps)){
		const int m = world.size(), n = world.front().length();
		auto access = [&](int i, int j) {
			if(i<0||i>=m||j<0||j>=n) return far(t);
			return world[i][j] == '#';
		};

		std::vector out(m+2, std::string(n+2,'.'));
		for(auto&& [i, j] : expand(m,n)) {
			int idx = 0;
			for(auto&& [ii, jj]: surrounding(i,j))
				idx = (idx<<1) | access(ii,jj);
			out[i+1][j+1] = IEA[idx];
		}

		world = out;
	}
}

size_t count_lit(const auto& world) {
	return rg::count(world|rv::join,'#');
}


int main(void) {
	const auto IEA = rg::getlines(std::cin) 
		| rv::take(1) | rv::join | rg::to<std::string>;
	auto world = rg::getlines(std::cin) | rg::to<std::vector<std::string>>;

	const auto far = [fst=IEA.front()=='#', end=IEA.back()=='#'](int t)->bool{
		return !fst? 0 : end ? (t==0) : (t%2);
	};

	enhance(world, far, IEA, 2);
	std::cout << "Answer 1: " << count_lit(world) << '\n';

	enhance(world, far, IEA, 50-2);
	std::cout << "Answer 2: " << count_lit(world) << '\n';
}
