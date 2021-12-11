#include <iostream>
#include <string>
#include <vector>
#include <range/v3/all.hpp>

static const int SIZE = 10;

namespace rg = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

auto read(std::istream& is) {
        return rg::istream<std::string>(is) 
             | rv::join 
             | rv::transform([](char c)->int{return c - '0';})
             | rv::take(SIZE*SIZE)
             | rg::to_vector;
}

template<int D>
auto valid_neighbors(int i, int j){
        return rv::cartesian_product(
                        rv::iota(std::max(i-D, 0), std::min(i+D+1, SIZE)), 
                        rv::iota(std::max(j-D, 0), std::min(j+D+1, SIZE)));
}

int main(int argc, const char* argv[]) {
        auto board{read(std::cin)};
        auto update = [](int& n)->int{ return (n<10)? ++n : n; };

        for(size_t t = 1, tot = 0; true; ++t){
                board |= ra::transform(update);
                size_t spreading = true;
                while(spreading){
                        spreading = false;
                        for(auto&& [idx, oct] : board | rv::enumerate){
                                if(oct != 10) continue;
                                for(auto [ii, jj] : valid_neighbors<1>(idx/SIZE, idx%SIZE)) {
                                        update(board[ii*SIZE+jj]);
                                }
                                oct = -1000;
                                spreading = true;
                        }
                }
                size_t flash = 0;
                for(auto& oct : board){
                        if(oct < 0){
                                flash ++;
                                oct = 0;
                        }
                }
                tot += flash;

                if(t == 101){
                        std::cout << "Answer 1: " << tot << '\n';
                }
                if (flash == SIZE*SIZE) {
                        std::cout << "Answer 2: " <<  t  << '\n';
                        break;
                }
        }
}

