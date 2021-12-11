#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <functional>

struct Line { const int x1, y1, x2, y2; };
using Input = std::vector<Line>;

size_t board_size(const auto& in) {
        using std::max;
        return std::transform_reduce(in.cbegin(), in.cend(), 
                        0,
                        [](auto a, auto b){return max(a,b);},
                        [](const auto& l){return max(max(l.x1,l.x2), 
                                                     max(l.y1,l.y2));}
        );
}

bool is_axial(const Line& l) {
        return (l.x1 == l.x2) || (l.y1 == l.y2);
}

bool is_diagonal(const Line& l) {
        return std::abs(l.x1-l.x2) == std::abs(l.y1-l.y2);
}

void draw_line(const auto& draw, const Line& l) {
        assert(is_axial(l) || is_diagonal(l));
        const auto& [x1, y1, x2, y2] = l;
        const int dx = (x2>x1)? 1 : (x2<x1)? -1 : 0;
        const int dy = (y2>y1)? 1 : (y2<y1)? -1 : 0;
        const int len = std::max(dx*(x2-x1), dy*(y2-y1));
        for(int t = 0, x=x1, y=y1; t <= len; x+=dx, y+=dy, t++)
                draw(x,y);
}

void draw_lines(const auto& draw, auto lines) {
        for(const auto& l: lines) draw_line(draw, l);
}

size_t count_overlap(const auto& b) {
        using namespace std;
        return ranges::count_if(b, bind_front(less<>{}, 1));
}


int main(void) {
        using std::views::filter;
        Input in;
        for(int x1,y1,x2,y2;
            std::scanf("%d,%d -> %d,%d ", &x1, &y1, &x2, &y2) != EOF;
            in.emplace_back(x1,y1,x2,y2));

        const auto N = board_size(in);
        std::vector<unsigned> board(N*N, 0);
        auto inc = [&](auto i, auto j){board[j*N+i]++;};

        draw_lines(inc, in | filter(is_axial));
        std::cout << "Answer 1: " << count_overlap(board) << '\n';

        draw_lines(inc, in | filter(is_diagonal));
        std::cout << "Answer 2: " << count_overlap(board) << '\n';
        return 0;
}
