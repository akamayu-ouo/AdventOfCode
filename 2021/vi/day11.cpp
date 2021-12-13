#include <iostream>
#include <string>
#include <vector>
#include <range/v3/all.hpp>

int SIZE = 10;

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

void set_color(float f) {
        int r = (f<1)? 255 : std::lerp(0,225, f/10);
        int g = (f<1)? 255 : std::lerp(0,225, f/10);
        int b = (f<1)? 100 : std::lerp(0,225, f/10);
        std::cout << "\x1b[48;2;" << r << ';' << g << ';' << b << "m";
}

const char* NUL = "\x1b[0m";
int main(int argc, const char* argv[]) {

        if(argc > 1)
                SIZE = std::stoi(argv[1]);

        auto board{read(std::cin)};
        auto update = [](int& n)->int{ return (n<10)? ++n : n; };
        auto show = [&]() {
                for(auto&& [idx, oct] : board | rv::enumerate){
                        set_color(oct);
                        std::cout << "  " << NUL;
                        if(idx%SIZE == (SIZE-1))     
                                std::putchar('\n');
                }
                std::putchar('\n');
        };

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

                std::cout << "\033c" ;
                show();
                std::cout << "Time       : " << t << '\n';
                std::cout << "New Flash  : " << flash << '\n';
                std::cout << "Total Flash: " << tot << '\n';
                usleep(100000);
                if(t == 101){
                        std::cout << "Answer 1: " << tot << '\n';
                }
                if (flash == SIZE*SIZE) {
                        std::cout << "Answer 2: " <<  t  << '\n';
                        break;
                }
        }
}

