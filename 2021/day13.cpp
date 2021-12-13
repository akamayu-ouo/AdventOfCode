#include <iostream>
#include <vector>
#include <string>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = rg::views;
namespace ra = rg::actions;

auto fold_num(int f, int& n) { n = f - std::abs(f - n); }

auto fold_along(const auto& f) {
	return [=](auto& p) {
		fold_num(f.second, (f.first=='x')? p.first 
					         : p.second);
		return p;
	};
}

void folding(auto& d, const auto& fs){
	for(const auto& f : fs) d |= ra::transform(fold_along(f));
	d |= ra::sort | ra::unique;
}

int main(void) {
	std::vector<std::pair<int,int>> dots{};
	std::vector<std::pair<char,int>> folds{};

	for(auto h : rg::getlines(std::cin)){
		if(h.length() == 0 ) break;
		auto m = h.find(',');
		auto a = std::stoi(h.substr(0,m));
		auto b = std::stoi(h.substr(m+1));
		dots.emplace_back(a, b);
	}

	for(auto h : rg::getlines(std::cin)){
		auto m = h.find('=');
		auto c = h[m-1];
		auto i = std::stoi(h.substr(m+1));
		folds.emplace_back(c, i);
	}

	folding(dots, folds | rv::take(1));

	std::cout << "Answer 1:" << dots.size() << '\n';

	folding(dots , folds | rv::drop(1));

        auto [xmin, xmax] = rg::minmax(dots | rv::keys);
        auto [ymin, ymax] = rg::minmax(dots | rv::values);

        auto xh = xmax - xmin + 1, yh = ymax - ymin + 1;
        std::vector paper(yh, std::string(xh, ' '));
        for(const auto& [x,y]: dots)
                paper[y-ymin][x-xmin] = '#';

	std::cout << "Answer 2:" << '\n';
	rg::copy(paper, rg::ostream_iterator<>(std::cout, "\n"));
}

