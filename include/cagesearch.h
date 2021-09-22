#ifndef __CAGE_SEARCH_HPP__
#define __CAGE_SEARCH_HPP__

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <ctime>
#include <fstream>
#include <strstream>
#include <iomanip>

#include "nauty.h"
#include "gtools.h"
#include "naugroup.h"

// #include "graph6.hpp"
#include "cmdline.h"

#define B(i) (1 << ((i)-1))
#define M(i) ((1 << (i)) - 1)

class CageSearcher
{
public:
    CageSearcher();
    graph *g;
    int im; // Size of store type, not meaningful for graph.
    int in; // Size of graph, number of nodes.
    void init_graph_from_str(std::string s);
    void output_graph();
    void output_graph(std::vector<std::vector<int>> &adjmat);
    void output_color_pattern(int numaddons, std::vector<int> &old_color);
    void output_color_pattern(int numaddons, std::vector<std::vector<int>> &colpair);
    void output_color_pattern(int numaddons, std::ofstream &output_color_pattern_to_file);
};
void stringtograph(std::string s, graph *g, int m);
int graphsize(std::string s);
static void gt_abort(char *msg);
static int scan_console(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges, long sofar, long numcols, grouprec *group, int m, int n, std::vector<int> &old_color);
static int trythisone2console(grouprec *group, graph *g, _Boolean digraph, int m, int n);
static int scan_stdvec(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges, long sofar, long numcols, grouprec *group, int m, int n, std::vector<std::vector<int>> &colpair);
static int trythisone2stdvec(grouprec *group, graph *g, _Boolean digraph, int m, int n, std::vector<std::vector<int>> &colpair);
static int scan_fstream(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges, long sofar, long numcols, grouprec *group, int m, int n, std::ofstream &output_color_pattern_to_file);
static int trythisone2fstream(grouprec *group, graph *g, _Boolean digraph, int m, int n, std::ofstream &output_color_pattern_to_file);

#endif
