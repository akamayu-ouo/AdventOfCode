#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>



int part1(const auto& v) {
        int h=0,d=0;
        for(const auto&[i,j]:v){
                switch(i){
                        case 'f': h+=j; break;
                        case 'u': d-=j; break;
                        case 'd': d+=j; break;
                        default: break;
                }
        }
        return h*d;
}

int part2(const auto& v) {
        int h=0,d=0,a=0;
        for(const auto&[i,j]:v){
                switch(i){
                        case 'f': h+=j, d+=a*j; break;
                        case 'u': a-=j; break;
                        case 'd': a+=j; break;
                        default: break;
                }
        }
        return h*d;
}


int main(void) {
        using In = std::pair<std::string,int>;
        using Ins = std::pair<char,int>;
        std::vector<Ins> v;
        for(In m; std::cin >> m.first >> m.second; v.emplace_back(m.first.front(),m.second));
        std::cout << "Answer1: " << part1(v) << '\n';
        std::cout << "Answer2: " << part2(v) << '\n';
        return 0;
}
