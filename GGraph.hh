#pragma once

#include <vector>


#define infinity std::numeric_limits<unsigned>::max()

using costT = std::tuple<unsigned, short>;

std::ostream& operator<<(std::ostream& out, const costT& c) {
	return out << std::get<0>(c) << ", " << std::get<1>(c);
}

struct edge {
	unsigned end;
	costT cost;

	edge(unsigned end, costT cost) :
		end(end), cost(cost) {}
	edge(unsigned end, unsigned distance, unsigned short depth) :
		end(end), cost(std::make_tuple(distance, depth)) {}
};

using graph = std::vector<std::vector<edge>>;

std::ostream& operator<<(std::ostream& out, const edge& e) {
	return out << e.end << " (" << e.cost << ") " << std::endl;
}

costT operator+(const costT& lhs, const costT& rhs) {
	return std::make_tuple(
		std::get<0>(lhs) + std::get<0>(rhs),
		std::get<1>(lhs) + std::get<1>(rhs)
	);
}
std::vector<std::tuple<unsigned, unsigned, bool>> * wybe(
                const std::vector<std::vector<std::tuple<bool, unsigned, unsigned>>>& roadSoFar,
                //const graph& g,
                unsigned short depth,
                const unsigned backtrack_from) {
	std::vector<std::tuple<unsigned, unsigned, bool>>* trail = new std::vector<std::tuple<unsigned, unsigned, bool>>;
	unsigned current;
	unsigned next = backtrack_from;
	trail->emplace_back(std::make_tuple(next, 0, false));
	while(next) {
		current = next;
                std::cout << "at node " << current << " depth " << depth << "\n";
		bool change_depth;
		unsigned cost;
		std::tie(change_depth, cost, next) = roadSoFar.at(current).at(depth);
		//change_depth = std::get<1>(g.at(current).at(next)) != 0;
		trail->emplace_back(std::make_tuple(next, cost, change_depth));
		if(change_depth)
			--depth;
	}

	return trail;
}

std::tuple<bool, std::vector<std::vector<unsigned>>, std::vector<std::vector<std::tuple<bool, unsigned, unsigned>>>, unsigned> // return tuple<distance, previous, finishing depth>
		edsger(const graph& g, unsigned max_node, unsigned max_depth, unsigned src, unsigned dest) {
        unsigned last_depth = 0;
	std::vector<std::vector<unsigned>> dist(max_node, std::vector<unsigned>(max_depth, infinity)); // switches, last type, depth

	std::vector<std::vector<std::tuple<bool, unsigned, unsigned>>> prev(max_node, std::vector<std::tuple<bool, unsigned, unsigned>>(max_depth+1, std::make_tuple(false, 0, 0))); // switches, last type, depth

	using costnode = std::tuple<costT, unsigned>;
	std::priority_queue<costnode, std::vector<costnode>, std::greater<costnode>> q; // depth, type, depth, node

	dist[src] = std::vector<unsigned>(max_depth, 0);
	q.emplace(costT(0, 0), src);

        #pragma GCC diagnostic push
        #ifndef _DEBUG_MORE
        #define dout(LVL) LVL >= 0 && std::cout
        #pragma GCC diagnostic ignored "-Wunused-value"
        #endif

	while(!q.empty()) {
		costT cost;
		unsigned node;
		std::tie(cost, node) = q.top(); q.pop();
		if(node == dest) {
			return std::make_tuple(true, dist, prev, last_depth);
		}
                dout(0) << "\nentering " << node << std::endl ;

		for(const auto& neigh_edge : g[node]) {
			const unsigned neigh = neigh_edge.end;
                        dout(0) << "neig: " << neigh << std::endl;

			for(unsigned short d = 0; d <= max_depth; d++) {
                                dout(0) << "depth: " << d << std::endl;
				if(dist[node][d] == infinity)
					continue;

				unsigned new_distance;
				unsigned short new_depth;
				std::tie(new_distance, new_depth) = std::make_tuple(dist[node][d], d) + neigh_edge.cost;

                                dout(0) << std::make_tuple(dist[node][d], d) << " + " <<neigh_edge.cost << "\n";

				if(new_depth > max_depth) {
                                        dout(0) << "we canno'nt go deeper " << new_distance << ", " << new_depth << "\n";
					continue;
                                }

				if(new_distance < dist[neigh][new_depth]) {
                                        dout(0) << "Well met! (from " << node << " to " << neigh << " is " << new_distance << " was " << dist[neigh][new_depth] <<")\n";

					dist[neigh][new_depth] = new_distance;
					prev[neigh][new_depth] = std::make_tuple(new_depth != d, new_distance, node);

					dout(1) << "insert " << neigh << " to queue\n";

					q.emplace(std::make_tuple(new_distance, new_depth), neigh);
                                        last_depth = new_depth;
				} else {
                                        dout(0) << "that didnt quite hit the mark (from " << node << " to " << neigh << " is " << dist[neigh][new_depth] << ")\n";
                                }
			}
		}
	}
        #undef dout
        #pragma GCC diagnostic pop
	return std::make_tuple(false, dist, prev, last_depth);
}

class GGraph {
private:
	graph es;
	unsigned max_node;
public:
	GGraph(unsigned max) : es(max), max_node(max) {}

	void add_edge(unsigned a, unsigned b, short costX, short costY) {
		es.at(a).emplace_back(b, costX, costY);
	}

	std::vector<std::tuple<unsigned, unsigned, bool>>* find_path(unsigned from, unsigned to, unsigned short depth) {
	        std::vector<std::vector<unsigned>> dist;
	        std::vector<std::vector<std::tuple<bool, unsigned, unsigned>>> prev;
		bool found;
		unsigned last_depth;
		//std::tuple<bool, std::vector<std::vector<unsigned>>, std::vector<std::vector<unsigned>>, unsigned> // return tuple<distance, previous, finishing depth>
		std::tie(found, dist, prev, last_depth ) =
			edsger(es, max_node, depth, from, to);
                std::cout << "edsger\n" << std::endl;

unsigned eta = 0;
                for(auto v0 : prev) {
			std::cout << ++eta << ":\t";
                        for(auto v1 : v0) {
                                std::cout << std::get<1>(v1) << (std::get<0>(v1) ? "\t-" : "\t ");
                        }
                        std::cout << "\n";
                }

		//throw 1;
		if(!found)
			new std::vector<std::tuple<unsigned, unsigned, bool>>;

		auto path = wybe(prev, last_depth, to);

		return path;
	}

        const graph& get_graph() const {
                return es;
        }
};

class GGraphBuilder {
private:
	std::vector<std::tuple<unsigned, unsigned, short, short> > buffer;
	unsigned max;
public:
	void add_edge(unsigned a, unsigned b, short costX, short costY) {
		max = std::max(max, std::max(a, b));
		buffer.emplace_back(a, b, costX, costY);
	}

	GGraph* build() {
		GGraph *gg = new GGraph(max+1);
		for(auto b : buffer) {
			gg->add_edge(
				std::get<0>(b),
				std::get<1>(b),
				std::get<2>(b),
				std::get<3>(b)
			);
		}
		return gg;
	}
	void clear() {
		buffer.clear();
		max = 0;
	}
};
