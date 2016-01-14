#include <iostream>

#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>
#include <queue>

#include <sstream>

#include "BiMap.cc"
#include "GGraph.hh"

//using namespace std;
/*
#define SELFAWARE_IDENTIFIER(NAME) \
	template<typename T> \
	struct NAME { \
		T NAME; \
		constexpr static char const *name() { return #NAME; } \
		using type = T; \
		T &value() & { return this->NAME; } \
		T const &value() const & { return this->NAME; } \
		T &&value() && { return this->NAME;  } \
	};

SELFAWARE_IDENTIFIER(e_end);
SELFAWARE_IDENTIFIER(e_cost);
SELFAWARE_IDENTIFIER(e_type);

template<typename...Fields>
struct Struct : Fields... {
	using struct_type = Struct;
	using tuple_type = std::tuple<typename Fields::type...>;

	Struct() = default;
	Struct(Struct const &) = default;

	//template<typename TT>
	//constexpr Struct(TT &&...x) : Fields{std::forward(x)}...{};
	constexpr Struct(typename Fields::type &&...x) : Fields{std::forward(x)}... {}
	constexpr Struct(typename Fields::type &...x) : Fields{x}... {}
	//constexpr Struct(typename Fields::Q_type &&...x) : Fields{x}... {}

};

template<typename...Fields>
auto toTuple(Struct<Fields...> const &s)
	-> std::tuple<typename Fields::type...> {
		return std::make_tuple(s.Fields::value()...);
}

template<typename...Fields>
void toTuple(Struct<Fields...> &s, std::tuple<typename Fields::type...> const &t) {
	std::tie(s.Fields::value()...) = t;
}

using NewEdge = Struct<e_end<unsigned>, e_cost<unsigned>, e_type<unsigned>>;
*/
//static_assert(std::is_trivial<NewEdge>::value, "should be trivial");


/*
#define COMPARE_LEXICALLY(L, R, V1, V2) std::tie(L.V1, L.V2) < std::tie(R.V1, R.V2);
bool operator<(const edge& lhs, const edge& rhs) {
	return COMPARE_LEXICALLY(lhs, rhs, end, cost);
}
*/


template<typename T>
class Maybe {
private:
        bool nothing = true;
        T value;
public:
        Maybe() : nothing(true) {}
        explicit Maybe(T v) : nothing(false), value(v) {};

        bool isJust() const { return !nothing; }
        bool isNothing() const { return nothing; }

        operator bool() const {
                return isJust();
        }

        T fromJust() const {
                if(isNothing())
                        throw "is Nothing!";
                return value;
        }
};

#include <fstream>
#include <set>
#include <string>

//using nodeToNodeMap = SomewhatBiMap<std::tuple<unsigned, short>, unsigned>;

//std::ostream& dot_graph(std::ostream& out, const graph& g) {//, const nodeToNodeMap& m) {
std::ostream& dot_graph(std::ostream& out, const graph& g, const SomewhatBiMap<std::tuple<bool, std::string>, unsigned>& m) {
        out << "digraph G {\nsplines=true\n";
        std::set<unsigned> ns;
        for(unsigned i = 0; i < g.size(); i++) {
                for(const auto& it : g[i]) {
                        ns.insert(i);
                        ns.insert(it.end);
			if(it.cost != std::make_tuple(-1, 0) && it.cost != std::make_tuple(0, 1))
	                        out << i << " -> " << it.end << " [label=\"(" << it.cost << ")\"];\n";
                }
        }
        for(unsigned n : ns) {
                auto nc = m.right_get(n);
                //gs.emplace_back(std::get<1>(nc), n);
                //const char *params = std::get<1>(nc) == -1 ? ",fillcolor=\"palegreen\",style=\"filled\"" : "";
                out << n << "[label=\"" << (std::get<0>(nc) ? "[A] " : "[T] ") << std::get<1>(nc) <<"("<<n<<")\"]\n";
        }
	/*
        sort(gs.begin(), gs.end());
        unsigned last = 0;
        bool first = true;
        out << "subgraph cluster_" << last <<" {\n style=filled;\n";
        for(auto g : gs) {
                if(g.first == last) {
                        if(!first)
                                out << ", ";
                        out << g.second;
                        first = false;
                } else {
                        last = g.first;
                        first = true;
                        out << "\n}\nsubgraph cluster_" <<last<<" {\nstyle=filled;\n" << g.second << ", ";
                }
        }
	*/
        //out << "\n}\n";
        return out<< "}\n";
}

#if 0
int main2(3 {
        nodeSequence ns;
        ns.next();
        nodeToNodeMap nm; // L = old, R = new

	unsigned edge_num;

	unsigned max_node = 100000;
	unsigned max_depth = 1;
        const unsigned max_types = 2;

        unsigned calculated_max = 0;

	std::stringstream ss;
	ss <<
	"6 "
	"1 2 3 0 0 "
	"2 3 4 0 0 "
	"2 3 1 0 0 "
	"1 3 2 0 0 "
	"3 2 2 0 0 "
	"3 1 1 0 0 "
	;
	std::istream& input = std::cin;

	input >> edge_num;

	graph es(max_node);
	for(unsigned i = 0; i < edge_num; i++) {
		unsigned e_start, e_end, e_costx, e_type;
                unsigned t_start, t_end;
                bool increment;

		input >> e_start >> e_end >> e_costx >> e_type;

                std::tie(increment, t_start) = nm.left_get(std::make_tuple(e_start, e_type), ns.current());
                if(increment)
                        ns.next();

                std::tie(increment, t_end) = nm.left_get(std::make_tuple(e_end, e_type), ns.current());
                if(increment)
                        ns.next();

		es.at(t_start).emplace_back(t_end, e_costx, 0);
                calculated_max = max(e_start, e_end) > calculated_max ? max(e_start, e_end) : calculated_max;
	}
        for(unsigned i = 1 ; i <= calculated_max; i++) {
                unsigned node = ns.next();
                //nm.insert(std::make_tuple(i, -1), node);
		for(unsigned t0 = 0; t0 < max_types; t0++)
	                for(unsigned t1 = 0; t1 < t0; t1++) {
	                        unsigned node_to;
	                        unsigned node_from;
	                        bool increment;

	                        std::tie(increment, node_to) = nm.left_get(std::make_tuple(i, t0), ns.current());
	                        if(increment)
	                                ns.next();
	                        std::tie(increment, node_from) = nm.left_get(std::make_tuple(i, t1), ns.current());
	                        if(increment)
	                                ns.next();

	                        //nm.insert(std::make_tuple(i, t), node_to);
	                        es.at(node_from).emplace_back(node_to, 0, 1);
	                        es.at(node_to).emplace_back(node_from, 0, 1);
	                }
        }



        std::vector<std::vector<unsigned>> dist, prev;
        unsigned last_depth;
	std::tie(dist, prev,last_depth) = edsger(es, max_node, max_depth, 1);

        for(unsigned i = 1; i < calculated_max; i++) {
                std::cout << "dist to : " << i << "\n";
                for(unsigned l = 0; l <= max_depth; l++) {
                        if(dist[i][l] != infinity)
                                std::cout << "\t d:" << l << " l: " << (dist[i][l]) << "\n";
                }
        }
	cout << "Foo" << std::endl;

	for(unsigned i = 1; i < calculated_max; i++) {
		std::cout << "prev of " << i <<"\n";
		for(unsigned l = 0; l <= max_depth; l++) {
			if(prev[i][l] != 0) {
				std::cout << "\t d: " << l << " p: " << prev[i][l] <<"\n";
			}
		}
	}

        std::ofstream dotfile{"graph.dot", ios::trunc};
        dot_graph(dotfile, es, nm);
        return 0;
}


int main2() {
	unsigned edge_num;

	unsigned max_node = 100000;
	unsigned max_depth = 1;

        unsigned calculated_max = 0;

	std::stringstream ss;
	ss <<
	"6 "
	"1 2 3 0 0 "
	"2 3 4 0 0 "
	"2 3 1 0 0 "
	"1 3 2 0 0 "
	"3 2 2 0 0 "
	"3 1 1 0 0 "
	;
	std::istream& input = std::cin;

	input >> edge_num;

	graph es(max_node);
	for(unsigned i = 0; i < edge_num; i++) {
		unsigned e_start, e_end, e_costx, e_costy;
		input >> e_start >> e_end >> e_costx >> e_costy;
		es.at(e_start).emplace_back(e_end, e_costx, e_costy);
                calculated_max = max(e_start, e_end) > calculated_max ? max(e_start, e_end) : calculated_max;
	}

        std::vector<std::vector<unsigned>> dist, prev;
        unsigned last_depth;
	std::tie(dist, prev,last_depth) = edsger(es, max_node, max_depth, 1);

        for(unsigned i = 1; i < calculated_max; i++) {
                std::cout << "dist to : " << i << "\n";
                for(unsigned l = 0; l <= max_depth; l++) {
                        if(dist[i][l] != infinity)
                                std::cout << "\t d:" << l << " l: " << (dist[i][l]) << "\n";
                }
        }
	cout << "Foo" << std::endl;

        std::ofstream dotfile{"graph.dot", ios::trunc};
        //dot_graph(dotfile, es);
}
#endif
