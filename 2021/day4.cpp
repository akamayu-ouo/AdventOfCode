#include <iostream>
#include <vector>
#include <optional>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <string>
#include <sstream>
#include <set>

using Marks = std::array<bool, 100>;
using Board = std::array<int, 25>;
using Boards = std::vector<Board>;

struct Input {
        std::vector<int> plays;
        Boards boards;
};

bool bingo(const Board& b, const Marks & m) {
        return std::ranges::any_of(BingoLines, [&](auto l){
                        for(unsigned i = 0; i < 5; ++i){
                                bool row{true}, col{true};
                                for(unsigned j = 0; j < 5; ++j){
                                        row &= m[b[i*5+j]];
                                        col &= m[b[j*5+i]];
                                }
                                if(row || col) return true;
                        }
                        return false;
                });
}

size_t sum_unmarked(const Board& b, const Marks& m) {
        return std::transform_reduce(b.cbegin(), b.cend(), 0UL
                        , std::plus<>{}, [&](int n){return m[n] ? 0 : n;});
}

size_t part1(const Input& in) {
        Marks mark{false};
        auto bingo_by_mark = [&](auto it){return bingo(it, mark);};
        for(auto n : in.plays){
                mark[n] = true;
                auto b = std::ranges::find_if(in.boards, bingo_by_mark);
                if(b != in.boards.cend())
                        return sum_unmarked(*b, mark) * n;
        }
        return 0;
}

size_t part2(const Input& in) {
        Marks mark{false};
        auto bingo_by_mark = [&](auto it){return bingo(*it, mark);};

        const auto& bds = in.boards;
        std::set<decltype(bds.cbegin())> unfinished{};
        for(auto it = bds.cbegin(); it != bds.cend(); it++)
                unfinished.insert(it);

        for(auto n : in.plays){
                mark[n] = true;
                auto it = std::ranges::find_if(unfinished, bingo_by_mark);
                if(it != unfinished.cend()){
                        if(unfinished.size() == 1)
                               return sum_unmarked(**it, mark) * n;
                        unfinished.erase(it);
                }
        }
        return 0;
}

int main(void) {
        Input in;
        std::string line;
        for(unsigned n = 0, row = 0; std::getline(std::cin, line); n++){
                std::istringstream iss{line};
                if(n == 0) {
                        std::string num;
                        while(std::getline(iss, num, ','))
                                in.plays.emplace_back(std::stoi(num));
                }else if(line.length() == 0){
                        in.boards.emplace_back();
                        row = 0;
                }else {
                        for(unsigned nn = 0; nn < 5; nn++)
                                iss >> in.boards.back()[row*5+nn];
                        row++;
                }
        }
        std::cout << "Answer1: " << part1(in) << '\n';
        std::cout << "Answer2: " << part2(in) << '\n';
        return 0;
}
