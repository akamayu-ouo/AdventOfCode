#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ranges>
using std::vector;
using std::string;
using In = vector<std::string>;

size_t part1(const auto& map){
	size_t m = map.size(), n = map.front().size();
	auto idx = [&](int i, int j){
		return (i < 0 || j < 0 || i >= m || j >= n)? 
			10 : (map[i][j]-'0');
	};
	size_t total = 0;
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			if (auto now = idx(i,j);
			    (idx(i-1, j) > now) &&
			    (idx(i+1, j) > now) &&
			    (idx(i, j-1) > now) &&
			    (idx(i, j+1) > now))
				total += now + 1;
		}
	}
	return total;
}

size_t basin_size(auto& ouo, int i, int j){
	size_t m = ouo.size(), n = ouo.front().size();
	if(i < 0 || i == m || j < 0 || j == n) return 0;
	if(ouo[i][j]) return 0;
	ouo[i][j] = true;
	return 1 + basin_size(ouo, i-1, j)
		 + basin_size(ouo, i+1, j)
		 + basin_size(ouo, i, j-1)
		 + basin_size(ouo, i, j+1);
}

size_t part2(const auto& map) {
	using namespace std::ranges;
	size_t m = map.size(), n = map.front().size();
	vector visited(m, vector<short>(n, 0));
	auto flood = [&](int i, int j){
		return basin_size(visited, i, j);};

	for(auto i = 0UL; i < m; ++i){
		transform(map[i], visited[i].begin(),
				[](char c){return c == '9';});
	}

	vector<int> sz{};
	for(auto i = 0UL; i < m; ++i){
		for(auto j = 0UL; j < n; ++j){
			if(!visited[i][j])
				sz.push_back(flood(i, j));
		}
	}

	sort(sz, std::greater<>{});
	return sz[0] * sz[1] * sz[2];
}

In read(std::istream& is) {
	In in;
	string line;
	while(std::getline(is, line))
		in.push_back(line);
	return in;
}

int main(void) {
	In in{read(std::cin)};
	std::cout << "Answer 1: " << part1(in) << '\n';
	std::cout << "Answer 2: " << part2(in) << '\n';
}

