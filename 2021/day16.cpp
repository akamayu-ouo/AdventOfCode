#include <iostream>
#include <vector>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct Packet {
        size_t version  {static_cast<size_t>(-1)};
        size_t type     {static_cast<size_t>(-1)};
        size_t value    { 0};
        std::vector<Packet> sub_packets{};
        Packet(){}
        Packet(size_t ve, size_t t, size_t va)
                : version{ve}, type{t}, value{va}{}
        Packet(size_t ve, size_t t, std::vector<Packet>&& p)
                : version{ve}, type{t}, sub_packets{std::move(p)}{}
};

template<size_t N>
auto read_bits(auto& rng) {
        auto ans = rg::accumulate(rng | rv::take(N), 0ULL,
                        [](auto&& a, auto&& b){return a*2+b;});
        rng.advance(N);
        return ans;
}

Packet decode(auto&& rng) {
        if(rng.empty()) return {};
        auto version{read_bits<3>(rng)};
        auto type{read_bits<3>(rng)};
        if(type == 4){
                size_t lit = 0ULL;
                size_t u ;
                do{
                        u = read_bits<5>(rng);
                        lit = (lit<<4)|(u&0b1111);
                }while(u>>4);
                return Packet(version, type, lit);
        } else {
                auto length_type = read_bits<1>(rng);
                std::vector<Packet> pkts{};
                if(length_type){
                        pkts = rv::generate_n([&]{return decode(rng);},
                                        read_bits<11>(rng))
                                | rg::to_vector;
                }else{
                        auto subs_len = read_bits<15>(rng);
                        auto subs = rv::counted(rng.begin(), subs_len);
                        pkts = rv::generate([&]{return decode(subs);})
                                | rv::take_while([&](auto&&p){return p.type <= 7;})
                                | rg::to_vector;
                        rng.advance(subs_len);
                }
                return Packet(version, type, std::move(pkts));
        }
}

size_t sum_version(const Packet& p){
        return rg::accumulate(p.sub_packets | rv::transform(sum_version), p.version);
}

size_t eval(const Packet& p) {
        auto reduce = [&](auto&& f, auto id){
                return rg::accumulate(p.sub_packets | rv::transform(eval), id, f);
        };
        auto diadic = [&](auto&& f) {
                return f(eval(p.sub_packets.front()), eval(p.sub_packets.back()));
        };
        switch(p.type){
                case 0: return reduce(rg::plus{}, 0ULL);
                case 1: return reduce(rg::multiplies{}, 1ULL);
                case 2: return reduce(rg::min, size_t(-1));
                case 3: return reduce(rg::max, size_t(0));
                case 4: return p.value;
                case 5: return diadic(rg::greater{});
                case 6: return diadic(rg::less{});
                case 7: return diadic(rg::equal_to{});
                default: break;
        }
        return p.value;
}

int main(void) {
        const auto dehex = [](char c){return (c >= 'A')? (c - 'A' + 10) : (c - '0');};
        const auto bits = rg::getlines(std::cin) | rv::join
                        | rv::transform([&](auto c){
                                return rg::yield_from(rv::iota(0,4)|rv::reverse|rv::transform(
                                                        [h=dehex(c)](auto i){ return ((1<<i)&h) != 0; }
                                                        ));
                                })
                        | rv::join | rv::const_ | rg::to_vector;
        auto p = decode(rg::subrange(bits));
        std::cout << "Answer 1: " << sum_version(p) << '\n';
        std::cout << "Answer 2: " << eval(p) << '\n';
}
