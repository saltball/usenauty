// link to nauty library (Apache License)
#include "cagesearch.h"

namespace cagesearch
{
    typedef unsigned long setword;
    typedef setword graph;

    CageSearcher::CageSearcher()
    {
    }

    void CageSearcher::init_graph_from_str(std::string s)
    {
        int reqm = 0;
        _Boolean digraph;
        const char *p;
        int m, n;
        int readg_code;

        if (s[0] == ':')
        {
            digraph = FALSE;
            p = s.data() + 1;
        }
        else if (s[0] == '&')
        {
            readg_code = DIGRAPH6;
            digraph = TRUE;
            p = s.data() + 1;
        }
        else
        {
            readg_code = GRAPH6;
            digraph = FALSE;
            p = s.data();
        }

        while (toascii(*p) >= BIAS6 && toascii(*p) <= MAXBYTE)
        {
            ++p;
        }
        if (*p == '\0')
            gt_abort(">E readgg: missing newline\n");
        else if (*p != '\n')
        {
            gt_abort(">E readgg: illegal character\n");
        }

        n = graphsize(s);

        if (readg_code == GRAPH6 && p - s.c_str() != G6LEN(n))
            gt_abort(">E readgg: truncated graph6 line\n");
        if (readg_code == DIGRAPH6 && p - s.c_str() != D6LEN(n))
            gt_abort(">E readgg: truncated digraph6 line\n");

        if (reqm > 0 && TIMESWORDSIZE(reqm) < n)
            gt_abort(">E readgg: reqm too small\n");
        else if (reqm > 0)
            m = reqm;
        else
            m = (n + WORDSIZE - 1) / WORDSIZE;

        if ((g = (graph *)calloc(n, m * sizeof(graph))) == NULL)
            gt_abort(">E readgg: malloc failed\n");

        in = n;
        im = m;

        stringtograph(s, g, m);
    }

    void CageSearcher::output_graph(void)
    {
        set *gi;
        int i, j;
        int m = im;
        int n = in;
        _Boolean first;

        for (i = 0, gi = (set *)g; i < n; ++i, gi += m)
        {
            first = TRUE;
            for (j = 0; j < n; ++j)
            {
                if (!first)
                    printf(" ");
                else
                    first = FALSE;
                if (ISELEMENT(gi, j))
                    printf("1");
                else
                    printf("0");
            }
            printf("\n");
        }
    }
    void CageSearcher::output_graph(std::vector<std::vector<int>> &adjmat)
    {
        set *gi;
        int i, j;
        int m = im;
        int n = in;

        _Boolean first;

        for (i = 0, gi = (set *)g; i < n; ++i, gi += m)
        {
            for (j = 0; j < n; ++j)
            {
                if (ISELEMENT(gi, j))
                    adjmat[i][j] = 1;
                else
                    adjmat[i][j] = 0;
            }
        }
    }


static void
stringtograph(std::string s, cagesearch::graph *g, int m)
/* Convert string (graph6, digraph6 or sparse6 format) to graph. */
/* Assumes g is big enough to hold it.   */
{
    const char *p;
    int n, i, j, k, v, x, nb, need;
    size_t ii;
    set *gi, *gj;
    _Boolean done;

    n = graphsize(s);
    if (n == 0)
        return;

    p = s.c_str() + (s.c_str()[0] == ':' || s.c_str()[0] == '&') + SIZELEN(n);

    if (TIMESWORDSIZE(m) < n)
        gt_abort(">E stringtograph: impossible m value\n");

    for (ii = m * (size_t)n; --ii > 0;)
        g[ii] = 0;
    g[0] = 0;

    if (s.c_str()[0] != ':' && s.c_str()[0] != '&') /* graph6 format */
    {
        k = 1;
        for (j = 1; j < n; ++j)
        {
            gj = GRAPHROW(g, j, m);

            for (i = 0; i < j; ++i)
            {
                if (--k == 0)
                {
                    k = 6;
                    x = *(p++) - BIAS6;
                }

                if ((x & TOPBIT6))
                {
                    gi = GRAPHROW(g, i, m);
                    ADDELEMENT(gi, j);
                    ADDELEMENT(gj, i);
                }
                x <<= 1;
            }
        }
    }
    else if (s.c_str()[0] == '&')
    {
        k = 1;
        for (i = 0; i < n; ++i)
        {
            gi = GRAPHROW(g, i, m);

            for (j = 0; j < n; ++j)
            {
                if (--k == 0)
                {
                    k = 6;
                    x = *(p++) - BIAS6;
                }

                if ((x & TOPBIT6))
                {
                    ADDELEMENT(gi, j);
                }
                x <<= 1;
            }
        }
    }
    else /* sparse6 format */
    {
        for (i = n - 1, nb = 0; i != 0; i >>= 1, ++nb)
        {
        }

        k = 0;
        v = 0;
        done = FALSE;
        while (!done)
        {
            if (k == 0)
            {
                x = *(p++);
                if (x == '\n' || x == '\0')
                {
                    done = TRUE;
                    continue;
                }
                else
                {
                    x -= BIAS6;
                    k = 6;
                }
            }
            if ((x & B(k)))
                ++v;
            --k;

            need = nb;
            j = 0;
            while (need > 0 && !done)
            {
                if (k == 0)
                {
                    x = *(p++);
                    if (x == '\n' || x == '\0')
                    {
                        done = TRUE;
                        continue;
                    }
                    else
                    {
                        x -= BIAS6;
                        k = 6;
                    }
                }
                if (need >= k)
                {
                    j = (j << k) | (x & M(k));
                    need -= k;
                    k = 0;
                }
                else
                {
                    k -= need;
                    j = (j << need) | ((x >> k) & M(need));
                    need = 0;
                }
            }
            if (done)
                continue;

            if (j > v)
                v = j;
            else if (v < n)
            {
                ADDELEMENT(GRAPHROW(g, v, m), j);
                ADDELEMENT(GRAPHROW(g, j, m), v);
            }
        }
    }
}

int graphsize(std::string s)
/* Get size of graph out of graph6, digraph6 or sparse6 string. */
{
    const char *p;
    int n;

    if (s.c_str()[0] == ':' || s.c_str()[0] == '&')
        p = s.c_str() + 1;
    else
        p = s.c_str();
    n = *p++ - BIAS6;
    if (n > SMALLN)
    {
        n = *p++ - BIAS6;
        if (n > SMALLN)
        {
            n = *p++ - BIAS6;
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
        else
        {
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
    }
    return n;
}

static void
gt_abort(char *msg) /* Write message and halt. */
{
    if (msg)
        std::cerr << msg;
    exit(1);
}
}