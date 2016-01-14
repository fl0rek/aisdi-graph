#include <iostream>
#include <regex>
#include "BiMap.hh"
#include "GGraph.hh"

#include "util.hh"

using DearMap = SomewhatBiMap<std::tuple<bool, std::string>, unsigned> ;
        // L = old, R = new
#define MINIMAL_CERR_LVL 0
#pragma GCC diagnostic push
#ifndef _DEBUG_MORE
#define cerr(LVL) cout; LVL >= MINIMAL_CERR_LVL && std::cout
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

int main() {
        unsigned max_depth;

	std::stringstream ss;

	ss <<
	"1 \n"
        "A Z\n"
	"\"A\", \"B\", 10,15 \n"
	"A,C,5,187\n"
        "A , D,34 ,12\n"
        "B,A, 0, 30\n"
        "D,C,12,-3\n"
        "  C,Z, 300,  200"
	;
/*
        ss <<
        "5 \nA B\n"
        "A, B, 10, 10";
*/
        std::istream &in = ss;

        std::string from, to;

        in >> max_depth;
        in >> from >> to;

        DearMap bg;

        std::regex \
                w_cudzys\u0141owie("^[[:space:]]*\"([^\"]+)\"[[:space:]]*,[[:space:]]*\"([^\"]+)\"[[:space:]]*,[[:space:]]*(-?[[:digit:]]+)[[:space:]]*,[[:space:]]*(-?[[:digit:]]+)[[:space:]]*$", std::regex::extended),
                jednak_bez_cudzys\u0141owia("[[:s:]]*([[:w:]]+)[[:s:]]*,[[:s:]]*([[:w:]]+)[[:s:]]*,[[:s:]]*(-?[[:digit:]]+)[[:s:]]*,[[:s:]]*(-?[[:digit:]]+)[[:s:]]*", std::regex::extended),
                prosz\u0118_cokolwiek("(.*),(.*),(.*),(.*)", std::regex::extended); // capture something and hope for the best

        GGraphBuilder ggb;
        std::string line;
        while(getline(in, line)) {
                std::regex gotoregex;
                std::cout << line << "\n";

                std::smatch sm;
                std::regex_match(line, sm, w_cudzys\u0141owie);
                if(sm.empty()) {
                        std::cerr(0) << "no qm match" << std::endl;
                        std::regex_match(line, sm, jednak_bez_cudzys\u0141owia);
                        if(sm.empty()) {
                                std::cerr(0) << "no noqm match, using fallback" << std::endl;
                                std::regex_match(line, sm, prosz\u0118_cokolwiek);
                                if(sm.empty()) {
                                        std::cerr(10) << "EE no match for line : ^" << line << "$"<< std::endl;
                                        continue;
                                }
                        }
                }

                std::string nodeA, nodeB;
                int a2b_T, a2b_A;
                nodeA = sm[1];
                nodeB = sm[2];


                a2b_T = std::stoi(sm[3]);
                a2b_A = std::stoi(sm[4]);

                std::cout << "\nn1: " << nodeA << " n2: " << nodeB << " t: " << a2b_T << " a: " << a2b_A <<std::endl;

                unsigned inodeA_A, inodeA_T,
                         inodeB_A, inodeB_T;

                inodeA_A = bg.register_left(std::make_tuple(true, nodeA)); //"A_" + nodeA);
                inodeB_A = bg.register_left(std::make_tuple(true, nodeB)); //"A_" + nodeB);

                inodeA_T = bg.register_left(std::make_tuple(false, nodeA)); //"T_" + nodeA);
                inodeB_T = bg.register_left(std::make_tuple(false, nodeB)); //"T_" + nodeB);

                ggb.add_edge(inodeA_A, inodeA_T, 0, 1);
                ggb.add_edge(inodeA_T, inodeA_A, 0, 1);

                ggb.add_edge(inodeB_A, inodeB_T, 0, 1);
                ggb.add_edge(inodeB_T, inodeB_A, 0, 1);

                if(a2b_T > 0) {
                        ggb.add_edge(inodeA_T, inodeB_T, a2b_T, 0);
                }
                if(a2b_A > 0) {
                        ggb.add_edge(inodeA_A, inodeB_A, a2b_A, 0);
                }
        }

        std::cout << "from: " << from << " to: " << to << std::endl;
        unsigned fromT, fromA,
                toT, toA;
        fromT = bg.left_get(std::make_tuple(false, from)); //"T_" + from);
        fromA = bg.left_get(std::make_tuple(true, from)); //"A_" + from);

        toT = bg.left_get(std::make_tuple(false, to)); //"T_" + to);
        toA = bg.left_get(std::make_tuple(true, to)); //"A_" + to);

        unsigned start, end;
        start = bg.register_left(std::make_tuple(true, "VIRTUAL_START")); //"VIRTUAL_START");
        end = bg.register_left(std::make_tuple(true, "VIRTUAL_END")); //"VIRTUAL_END");

        ggb.add_edge(start, fromT, 0, 0);
        ggb.add_edge(start, fromA, 0, 0);

        ggb.add_edge(toA, end, 0, 0);
        ggb.add_edge(toT, end, 0, 0);

        GGraph* g = ggb.build();
        ggb.clear();

        std::ofstream gwz{"graph.dot", std::ios::trunc};
        dot_graph(gwz, g->get_graph(), bg);
        gwz.close();

        //std::vector<std::tuple<unsigned, unsigned, bool>>* path;
        unsigned cost;

        auto path = g->find_path(start, end, max_depth+1);
        ///std::cout << "path " << (found ? "found" : "not found") << "\n";
        for(auto it = path->begin(); it != path->end(); it++) {
                std::tuple<unsigned, unsigned, bool> n = *it;
                //auto n = *it;
                std::cout << " - " << std::get<0>(n) << "\t[" << std::get<1>(n) << "]\t" << (std::get<2>(n) ? "changing" : "not changing") << "\n";
        }
        delete path;

}

#undef cout
#pragma GCC diagnostic pop
