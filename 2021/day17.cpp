#include <iostream>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = ranges::views;

struct Target {
        const int x0,x1,y0,y1;
        bool in_range(int x, int y) const { return x0 <= x && x <= x1 && y0 >= y && y >= y1; }
        auto vy_rng() const { return rv::iota(y1, x1/2+1); }
        auto vx_rng() const { return rv::iota((int)std::floor(std::sqrt(x0)), x1+1); }
        auto v_rng() const { return rv::cartesian_product(vy_rng(), vx_rng()) ; }
};

bool pass(Target t, std::tuple<int,int> v) {
        auto [vy, vx] = v;
        int y = 0, x = 0;
        while(y >= t.y1 && x <= t.x1) {
                if(t.in_range(x,y)) return true;
                x += vx;
                y += vy;
                vx -= (vx!=0);
                vy -= 1;
        }
        return false;
}

size_t part1(Target t){
        // The max height only depends on vy, so we just have to find
        // the biggest vy that hit the target with at least one vx.
        auto vs = t.v_rng() | rv::reverse; 
        auto [vy, vx] = *rg::find_if(vs, std::bind_front(pass, t));
        return (vy+1)*vy/2;
}

size_t part2(Target t) {
        return rg::count_if(t.v_rng(), std::bind_front(pass, t));
}

int main(void) {
        Target t = []{
                int x0, x1, y0, y1;
                std::scanf("target area: x=%d..%d, y=%d..%d", &x0, &x1, &y1, &y0);
                return Target{x0,x1,y0,y1};
        }();
        std::cout << "Answer 1:" << part1(t) << '\n';
        std::cout << "Answer 2:" << part2(t) << '\n';
        return 0;
}

