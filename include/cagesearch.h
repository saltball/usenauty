#ifndef __CAGE_SEARCH_HPP__
#define __CAGE_SEARCH_HPP__

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

typedef int _Boolean;
typedef unsigned long setword;
typedef setword set;

#define BIAS6 63
#define MAXBYTE 126
#define SMALLN 62
#define SMALLISHN 258047
#define TOPBIT6 32
#define C6MASK 63

#define FALSE 0
#define TRUE 1

#define B(i) (1 << ((i)-1))
#define M(i) ((1 << (i)) - 1)

#define SIZELEN(n) ((n) <= SMALLN ? 1 : ((n) <= SMALLISHN ? 4 : 8))
/* length of size code in bytes */
#define G6BODYLEN(n) \
    (((size_t)(n) / 12) * ((size_t)(n)-1) + (((size_t)(n) % 12) * ((size_t)(n)-1) + 11) / 12)
#define G6LEN(n) (SIZELEN(n) + G6BODYLEN(n))
/* exact graph6 string length excluding \n\0 
     This twisted expression works up to n=227023 in 32-bit arithmetic
     and for larger n if size_t has 64 bits.  */
#define D6BODYLEN(n) \
    ((n) * (size_t)((n) / 6) + (((n) * (size_t)((n) % 6) + 5) / 6))
#define D6LEN(n) (1 + SIZELEN(n) + D6BODYLEN(n))
/* exact digraph6 string length excluding \n\0 
     This twisted expression works up to n=160529 in 32-bit arithmetic
     and for larger n if size_t has 64 bits.  */

#define TIMESWORDSIZE(w) ((w) << 5)
#define SETWD(pos) ((pos) >> 5)
#define SETBT(pos) ((pos)&037)
#define ISELEMENT(setadd, pos) (((setadd)[SETWD(pos)] & bit[SETBT(pos)]) != 0)
#define ADDELEMENT(setadd, pos) ((setadd)[SETWD(pos)] |= bit[SETBT(pos)])
#define GRAPHROW(g, v, m) ((set *)(g) + (long)(v) * (long)(m))

#define WORDSIZE 32

#define GRAPH6_HEADER ">>graph6<<"
#define SPARSE6_HEADER ">>sparse6<<"
#define DIGRAPH6_HEADER ">>digraph6<<"

#define GRAPH6 1
#define SPARSE6 2
#define DIGRAPH6 4
#define UNKNOWN_TYPE 256
#define HAS_HEADER 512

#define EXTDEF_CLASS static
#define EXTDEF_TYPE 2

#if EXTDEF_TYPE == 1
extern setword bit[];
extern int bytecount[];
extern int leftbit[];

#else
/* array giving setwords with single 1-bit */
#if WORDSIZE == 64
#ifdef SETWORD_LONGLONG
EXTDEF_CLASS const
    setword bit[] = {01000000000000000000000LL, 0400000000000000000000LL,
                     0200000000000000000000LL, 0100000000000000000000LL,
                     040000000000000000000LL, 020000000000000000000LL,
                     010000000000000000000LL, 04000000000000000000LL,
                     02000000000000000000LL, 01000000000000000000LL,
                     0400000000000000000LL, 0200000000000000000LL,
                     0100000000000000000LL, 040000000000000000LL,
                     020000000000000000LL, 010000000000000000LL,
                     04000000000000000LL, 02000000000000000LL,
                     01000000000000000LL, 0400000000000000LL, 0200000000000000LL,
                     0100000000000000LL, 040000000000000LL, 020000000000000LL,
                     010000000000000LL, 04000000000000LL, 02000000000000LL,
                     01000000000000LL, 0400000000000LL, 0200000000000LL,
                     0100000000000LL, 040000000000LL, 020000000000LL, 010000000000LL,
                     04000000000LL, 02000000000LL, 01000000000LL, 0400000000LL,
                     0200000000LL, 0100000000LL, 040000000LL, 020000000LL,
                     010000000LL, 04000000LL, 02000000LL, 01000000LL, 0400000LL,
                     0200000LL, 0100000LL, 040000LL, 020000LL, 010000LL, 04000LL,
                     02000LL, 01000LL, 0400LL, 0200LL, 0100LL, 040LL, 020LL, 010LL,
                     04LL, 02LL, 01LL};
#else
EXTDEF_CLASS const
    setword bit[] = {01000000000000000000000, 0400000000000000000000,
                     0200000000000000000000, 0100000000000000000000,
                     040000000000000000000, 020000000000000000000,
                     010000000000000000000, 04000000000000000000,
                     02000000000000000000, 01000000000000000000,
                     0400000000000000000, 0200000000000000000,
                     0100000000000000000, 040000000000000000, 020000000000000000,
                     010000000000000000, 04000000000000000, 02000000000000000,
                     01000000000000000, 0400000000000000, 0200000000000000,
                     0100000000000000, 040000000000000, 020000000000000,
                     010000000000000, 04000000000000, 02000000000000,
                     01000000000000, 0400000000000, 0200000000000, 0100000000000,
                     040000000000, 020000000000, 010000000000, 04000000000,
                     02000000000, 01000000000, 0400000000, 0200000000, 0100000000,
                     040000000, 020000000, 010000000, 04000000, 02000000, 01000000,
                     0400000, 0200000, 0100000, 040000, 020000, 010000, 04000,
                     02000, 01000, 0400, 0200, 0100, 040, 020, 010, 04, 02, 01};
#endif
#endif

#if WORDSIZE == 32
EXTDEF_CLASS const
    setword bit[] = {020000000000, 010000000000, 04000000000, 02000000000,
                     01000000000, 0400000000, 0200000000, 0100000000, 040000000,
                     020000000, 010000000, 04000000, 02000000, 01000000, 0400000,
                     0200000, 0100000, 040000, 020000, 010000, 04000, 02000, 01000,
                     0400, 0200, 0100, 040, 020, 010, 04, 02, 01};
#endif

#if WORDSIZE == 16
EXTDEF_CLASS const
    setword bit[] = {0100000, 040000, 020000, 010000, 04000, 02000, 01000, 0400, 0200,
                     0100, 040, 020, 010, 04, 02, 01};
#endif

/*  array giving number of 1-bits in bytes valued 0..255: */
EXTDEF_CLASS const int bytecount[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                                      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

/* array giving position (1..7) of high-order 1-bit in byte: */
EXTDEF_CLASS const int leftbit[] = {8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
                                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif /* EXTDEFS */

namespace cagesearch
{
    typedef unsigned long setword;
    typedef setword graph;

    class CageSearcher
    {
    public:
        CageSearcher();
        graph *g;
        int im;
        int in;
        void init_graph_from_str(std::string s);
        void output_graph();
        void output_graph(std::vector<std::vector<int>> &adjmat);
    };
    void stringtograph(std::string s, cagesearch::graph *g, int m);
    int graphsize(std::string s);
    static void gt_abort(char *msg);

}

#endif
