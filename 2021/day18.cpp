#include <iostream>
#include <range/v3/all.hpp>

namespace rg = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct Node {
        Node* parent{nullptr};
        int number{0}, depth{0};
        Node* left{nullptr}, *right{nullptr};

        Node(Node* l, Node* r, Node* p = nullptr, int num = 0) 
                : parent{p? p : this}
		, number{num}
                , depth{0}
                , left{l}, right{r} {
			if(l) l->parent = this;
			if(r) r->parent = this;
                        if(l||r)redepth();
                }

        Node(Node* p) : Node(nullptr, nullptr, p){}

        Node(int n, Node* p = nullptr)
                : parent{p? p : this}
                , number{n}
                , depth{p? p->depth+1:0}
                , left{nullptr}, right{nullptr}{};

        Node* next() {
                Node* p = this;
                while(p->parent->right == p) {
                        p = p->parent;
                }
                if(p->is_root()) return nullptr;
                p = p->parent->right;
                while(!p->is_leaf()) {
                        p = p->left;
                }
                return p;
        }

        Node* prev() {
                Node* p = this;
                while(p->parent->left == p) {
                        p = p->parent;
                }
                if(p->is_root()) return nullptr;
                p = p->parent->left;
                while(!p->is_leaf()) {
                        p = p->right;
                }
                return p;
        }

	bool can_explode() const { return is_pair() && depth >= 4; }
        bool try_explode() {
                if(!can_explode()) return false;
                if(Node* nxt = this->right->next()) {
                        nxt->number += this->right->number;
                }
                if(Node *pre = this->left->prev()) {
                        pre->number += this->left->number;
                }
                delete this->right;
                delete this->left;
                this->number = 0;
                this->right = this->left = nullptr;
                return true;
        }

	bool can_split() const { return is_leaf() && number >= 10; }
        bool try_split() {
                if(!can_split()) return false;
                int a = number/2, b = number - a;
                this->number = 0;
                this->left = new Node(a, this);
                this->right = new Node(b, this);
                return true;
        }

        void redepth(int d = 0) {
		depth = d++;
                if(!is_leaf()){
                        left->redepth(d);
                        right->redepth(d);
                }
        }

        template<bool suicide>
        size_t magnitude() {
                auto ans = is_leaf()? number 
                        	    : 3*left->magnitude<suicide>() 
                        	    + 2*right->magnitude<suicide>();
                if constexpr(suicide){
                        delete this;
                }
                return ans;
        }

        bool is_leaf() const { return !left && !right; }

        bool is_pair() const { return !is_leaf() && (left->is_leaf() && right->is_leaf()); }

        bool is_root() const { return this->parent == this; }
};

Node* build(std::string_view& in) {
        if(!in.length()) return nullptr;
        if(in.front() == '['){
                in = in.substr(1); // Read '['
		auto l = build(in);
                in = in.substr(1); // Read ','
		auto r = build(in);
                in = in.substr(1); // Read ']'
        	return new Node(l, r);
        } else {
		auto c = in.front() - '0';
                in = in.substr(1);
		return new Node(c);
        }
}


void show(const Node* const n, int d = 0);

Node* apply_until(Node *r, auto&& f) {
        Node* now = [=]()mutable{
                while(r && !r->is_leaf()) r = r->left;
                return r;
        }();
        while(now && !f(now)){ now = now->next(); }
        return now;
}

Node* reduce(Node* r) {
        static const auto explode = [](Node* r){
                return apply_until(r, [](Node* n)->bool{return n->parent->try_explode();});
        };
        static const auto split = [](Node* r){
                return apply_until(r, [](Node* n)->bool{return n->try_split();});
        };
        while(explode(r) || split(r)){
		//show(r);
	}
        return r;
}

Node* copy_tree(const Node* const t) {
        if(!t) return nullptr;
        if(t->is_leaf()){
                return new Node(t->number);
        }else{
                return new Node(copy_tree(t->left) , copy_tree(t->right));
        }
}

Node* plus_node(Node* lhs, const Node* const rhs) {
        assert(rhs);
        auto* crhs = copy_tree(rhs);
        return !lhs? crhs : reduce(new Node(lhs, crhs));
}

size_t part1(const auto& in) {
        Node* result = rg::accumulate(in, static_cast<Node*>(nullptr), plus_node);
        return result->magnitude<true>();
}

size_t part2(const auto& in) {
        return rg::max(rv::cartesian_product(in, in)
                        | rv::filter([](auto&& tup){ 
                                auto [a, b] = tup; 
                                return a && b && (a!=b); })
                        | rv::transform([](auto&& tup)->size_t{ 
                                auto [a, b] = tup; 
                                Node* aa = copy_tree(a);
                                const Node* const bb = b;
                                return plus_node(aa,bb)->magnitude<true>(); })
                      );
}


int main(void) {
        const auto numbers = rg::getlines(std::cin) 
                | rv::transform([](std::string_view s){return reduce(build(s));}) 
                | rg::to<std::vector<const Node*>>;
        std::cout << "Answer 1: " << part1(numbers) << '\n';
        std::cout << "Answer 2: " << part2(numbers) << '\n';
        return 0;
}

void show(const Node* const n, int d) {
        static const auto set_bg = [](int r, int g, int b){
                std::cout << "\x1b[48;2;" << r << ';' << g << ';' << b << "m";
        };
        static const auto set_fg = [](int r, int g, int b){
                std::cout << "\x1b[38;2;" << r << ';' << g << ';' << b << "m";
        };
        static const auto clear_color = []{
                std::cout << "\x1b[0m";
        };
        if(!n) return ;
        if(!n->is_leaf()){
                if(n->can_explode()) set_bg(255,255,0);
                std::cout << '[';
                show(n->left, d+1);
                if(n->can_explode()) set_bg(255,255,0);
                std::cout << ',';
                show(n->right, d+1);
                if(n->can_explode()) set_bg(255,255,0);
                std::cout << ']';
                if(n->can_explode()) clear_color();
                if(d == 0) std::cout << '\n';
        }else{
                if(n->can_split()) set_fg(255,0,0);
                std::cout << n->number;
                if(n->can_split()) clear_color();
        }
        return;
}

