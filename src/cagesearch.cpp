// link to nauty library (Apache License)
#include "cagesearch.h"

// #define MAXARG 2000000000L
// #define NOLIMIT (MAXARG + 31L)
// #define TLS_ATTR
// #define DYNALLSTAT(type, name, name_sz) \
//     static TLS_ATTR type *name;         \
//     static TLS_ATTR size_t name_sz = 0
// DYNALLSTAT(int, id, id_sz);
// DYNALLSTAT(int, allp, allp_sz);
// DYNALLSTAT(set, workset, workset_sz);
// DYNALLSTAT(int, workperm, workperm_sz);
// DYNALLSTAT(int, bucket, bucket_sz);
// DYNALLSTAT(set, dnwork, dnwork_sz);

#define MAXNV 128
static _Boolean first;
static int col[MAXNV];
static unsigned long groupsize;
static unsigned long newgroupsize;
static int lastreject[MAXNV];
static _Boolean lastrejok;
static int fail_level;
// typedef unsigned long setword;
// typedef setword graph;
nauty_counter vc_nin, vc_nout;

CageSearcher::CageSearcher()
{
}

// convert graph6 format string (without header) to graph.
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
        throw -1;
        return;
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
    // if (*p == '\0')
    //     gt_abort(">E readgg: missing newline\n");
    // else if (*p != '\n')
    // {
    //     gt_abort(">E readgg: illegal character\n");
    // }

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

// print graph adjacent matrix to stdout.
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
                std::cout << " ";
            else
                first = FALSE;
            if (ISELEMENT(gi, j))
                std::cout << "1";
            else
                std::cout << "0";
        }
        std::cout << std::endl;
    }
}
void CageSearcher::output_graph(std::vector<std::vector<int>> &adjmat)
{
    set *gi;
    int i, j;
    int m = im;
    int n = in;

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

void CageSearcher::output_color_pattern(int numaddons, std::vector<int> &old_color)
{
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;
    setword workspace[MAXNV];
    grouprec *group;
    int i, j, k, nloops;
    set *gi, *gj;
    int lab[MAXNV], ptn[MAXNV], orbits[MAXNV];
    _Boolean loop[MAXNV];
    int prev[MAXNV]; /* If >= 0, earlier point that must have greater colour */
    int weight[MAXNV];
    int region, start, stop;
    int nfixed = 0;
    int minedges, maxedges;

    minedges = numaddons;
    maxedges = numaddons;
    
    
    int numcols = 2;

    if (in > MAXNV)
        gt_abort("Error: MAXNV exceeded\n");
    // nauty_check(WORDSIZE, im, in, NAUTYVERSIONID);

    nloops = 0;
    for (i = 0, gi = g; i < in; ++i, gi += im)
        if (ISELEMENT(gi, i))
        {
            DELELEMENT(gi, i);
            loop[i] = TRUE;
            ++nloops;
        }
        else
            loop[i] = FALSE;

    for (region = 0; region < 2; ++region)
    {
        if (region == 0)
        {
            if (nfixed == 0)
                continue;
            start = 0;
            stop = nfixed;
            if (stop > in)
                stop = in;
        }
        else
        {
            if (nfixed >= in)
                continue;
            start = nfixed;
            stop = in;
        }

        for (i = start, gi = g + im * (size_t)start; i < stop; ++i, gi += im)
        {
            /* Find most recent equivalent j. */
            for (j = i - 1, gj = gi - im; j >= start; --j, gj -= im)
            {
                if (loop[j] != loop[i])
                    continue;
                for (k = 0; k < im; ++k)
                    if (gi[k] != gj[k])
                        break;
                if (k < im)
                {
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                    for (k = 0; k < im; ++k)
                        if (gi[k] != gj[k])
                            break;
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                }
                if (k == im)
                    break;
            }
            if (j >= start)
            {
                prev[i] = j;
                weight[i] = weight[j] + 1;
            }
            else
            {
                prev[i] = -1;
                weight[i] = 0;
            }
        }
    }

    if (in == 0)
    {
        scan_console(0, g, FALSE, prev, minedges, maxedges, 0, numcols, FALSE, im, in, old_color);
        return;
    }

    for (i = nfixed; i < in; ++i)
        weight[i] += nfixed;

    if (maxedges == NOLIMIT || maxedges > in * numcols)
        maxedges = in * numcols;
    if (in * numcols < minedges)
        return;

    options.userautomproc = groupautomproc;
    options.userlevelproc = grouplevelproc;
    options.defaultptn = FALSE;
    options.digraph = (nloops > 0);

    setlabptn(weight, lab, ptn, in);

    if (nloops > 0)
        for (i = 0, gi = g; i < in; ++i, gi += im)
            if (loop[i])
                ADDELEMENT(gi, i);

    nauty(g, lab, ptn, NULL, orbits, &options, &stats, workspace, MAXNV, im, in, NULL);

    if (stats.grpsize2 == 0)
        groupsize = stats.grpsize1 + 0.1;
    else
        groupsize = 0;

    group = groupptr(FALSE);
    makecosetreps(group);

    if (stats.numorbits < in)
    {
        j = in;
        for (i = 0; i < in; ++i)
            if (orbits[i] < i && orbits[i] < j)
                j = orbits[i];

        for (i = j + 1; i < in; ++i)
            if (orbits[i] == j)
                prev[i] = j;
    }

    lastrejok = FALSE;

    for (i = 0; i < in; ++i)
        col[i] = 0;

    //for (int i = 0; i < old_color.size(); i++)
    //{
    //    col[old_color[i]] = 1;      
    //}

    scan_console(0, g, FALSE, prev, minedges, maxedges, 0, numcols, group, im, in, old_color);
}

void CageSearcher::output_color_pattern(int numaddons, std::ofstream &output_color_pattern_to_file)
{
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;
    setword workspace[MAXNV];
    grouprec *group;
    int i, j, k, nloops;
    set *gi, *gj;
    int lab[MAXNV], ptn[MAXNV], orbits[MAXNV];
    _Boolean loop[MAXNV];
    int prev[MAXNV]; /* If >= 0, earlier point that must have greater colour */
    int weight[MAXNV];
    int region, start, stop;
    int nfixed = 0;
    int minedges, maxedges;

    minedges = numaddons;
    maxedges = numaddons;

    int numcols = 2;

    if (in > MAXNV)
        gt_abort("Error: MAXNV exceeded\n");
    // nauty_check(WORDSIZE, im, in, NAUTYVERSIONID);

    nloops = 0;
    for (i = 0, gi = g; i < in; ++i, gi += im)
        if (ISELEMENT(gi, i))
        {
            DELELEMENT(gi, i);
            loop[i] = TRUE;
            ++nloops;
        }
        else
            loop[i] = FALSE;

    for (region = 0; region < 2; ++region)
    {
        if (region == 0)
        {
            if (nfixed == 0)
                continue;
            start = 0;
            stop = nfixed;
            if (stop > in)
                stop = in;
        }
        else
        {
            if (nfixed >= in)
                continue;
            start = nfixed;
            stop = in;
        }

        for (i = start, gi = g + im * (size_t)start; i < stop; ++i, gi += im)
        {
            /* Find most recent equivalent j. */
            for (j = i - 1, gj = gi - im; j >= start; --j, gj -= im)
            {
                if (loop[j] != loop[i])
                    continue;
                for (k = 0; k < im; ++k)
                    if (gi[k] != gj[k])
                        break;
                if (k < im)
                {
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                    for (k = 0; k < im; ++k)
                        if (gi[k] != gj[k])
                            break;
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                }
                if (k == im)
                    break;
            }
            if (j >= start)
            {
                prev[i] = j;
                weight[i] = weight[j] + 1;
            }
            else
            {
                prev[i] = -1;
                weight[i] = 0;
            }
        }
    }

    if (in == 0)
    {
        scan_fstream(0, g, FALSE, prev, minedges, maxedges, 0, numcols, FALSE, im, in, output_color_pattern_to_file);
        return;
    }

    for (i = nfixed; i < in; ++i)
        weight[i] += nfixed;

    if (maxedges == NOLIMIT || maxedges > in * numcols)
        maxedges = in * numcols;
    if (in * numcols < minedges)
        return;

    options.userautomproc = groupautomproc;
    options.userlevelproc = grouplevelproc;
    options.defaultptn = FALSE;
    options.digraph = (nloops > 0);

    setlabptn(weight, lab, ptn, in);

    if (nloops > 0)
        for (i = 0, gi = g; i < in; ++i, gi += im)
            if (loop[i])
                ADDELEMENT(gi, i);

    nauty(g, lab, ptn, NULL, orbits, &options, &stats, workspace, MAXNV, im, in, NULL);

    if (stats.grpsize2 == 0)
        groupsize = stats.grpsize1 + 0.1;
    else
        groupsize = 0;

    group = groupptr(FALSE);
    makecosetreps(group);

    if (stats.numorbits < in)
    {
        j = in;
        for (i = 0; i < in; ++i)
            if (orbits[i] < i && orbits[i] < j)
                j = orbits[i];

        for (i = j + 1; i < in; ++i)
            if (orbits[i] == j)
                prev[i] = j;
    }

    lastrejok = FALSE;
    for (i = 0; i < in; ++i)
        col[i] = 0;
    scan_fstream(0, g, FALSE, prev, minedges, maxedges, 0, numcols, group, im, in, output_color_pattern_to_file);
}

void CageSearcher::output_color_pattern(int numaddons, std::vector<std::vector<int>> &colpair)
{
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;
    setword workspace[MAXNV];
    grouprec *group;
    int i, j, k, nloops;
    set *gi, *gj;
    int lab[MAXNV], ptn[MAXNV], orbits[MAXNV];
    _Boolean loop[MAXNV];
    int prev[MAXNV]; /* If >= 0, earlier point that must have greater colour */
    int weight[MAXNV];
    int region, start, stop;
    int nfixed = 0;
    int minedges, maxedges;

    minedges = numaddons;
    maxedges = numaddons;

    int numcols = 2;

    if (in > MAXNV)
        gt_abort("Error: MAXNV exceeded\n");
    // nauty_check(WORDSIZE, im, in, NAUTYVERSIONID);

    nloops = 0;
    for (i = 0, gi = g; i < in; ++i, gi += im)
        if (ISELEMENT(gi, i))
        {
            DELELEMENT(gi, i);
            loop[i] = TRUE;
            ++nloops;
        }
        else
            loop[i] = FALSE;

    for (region = 0; region < 2; ++region)
    {
        if (region == 0)
        {
            if (nfixed == 0)
                continue;
            start = 0;
            stop = nfixed;
            if (stop > in)
                stop = in;
        }
        else
        {
            if (nfixed >= in)
                continue;
            start = nfixed;
            stop = in;
        }

        for (i = start, gi = g + im * (size_t)start; i < stop; ++i, gi += im)
        {
            /* Find most recent equivalent j. */
            for (j = i - 1, gj = gi - im; j >= start; --j, gj -= im)
            {
                if (loop[j] != loop[i])
                    continue;
                for (k = 0; k < im; ++k)
                    if (gi[k] != gj[k])
                        break;
                if (k < im)
                {
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                    for (k = 0; k < im; ++k)
                        if (gi[k] != gj[k])
                            break;
                    FLIPELEMENT(gi, i);
                    FLIPELEMENT(gj, j);
                }
                if (k == im)
                    break;
            }
            if (j >= start)
            {
                prev[i] = j;
                weight[i] = weight[j] + 1;
            }
            else
            {
                prev[i] = -1;
                weight[i] = 0;
            }
        }
    }

    if (in == 0)
    {
        scan_stdvec(0, g, FALSE, prev, minedges, maxedges, 0, numcols, FALSE, im, in, colpair);
        return;
    }

    for (i = nfixed; i < in; ++i)
        weight[i] += nfixed;

    if (maxedges == NOLIMIT || maxedges > in * numcols)
        maxedges = in * numcols;
    if (in * numcols < minedges)
        return;

    options.userautomproc = groupautomproc;
    options.userlevelproc = grouplevelproc;
    options.defaultptn = FALSE;
    options.digraph = (nloops > 0);

    setlabptn(weight, lab, ptn, in);

    if (nloops > 0)
        for (i = 0, gi = g; i < in; ++i, gi += im)
            if (loop[i])
                ADDELEMENT(gi, i);

    nauty(g, lab, ptn, NULL, orbits, &options, &stats, workspace, MAXNV, im, in, NULL);

    if (stats.grpsize2 == 0)
        groupsize = stats.grpsize1 + 0.1;
    else
        groupsize = 0;

    group = groupptr(FALSE);
    makecosetreps(group);

    if (stats.numorbits < in)
    {
        j = in;
        for (i = 0; i < in; ++i)
            if (orbits[i] < i && orbits[i] < j)
                j = orbits[i];

        for (i = j + 1; i < in; ++i)
            if (orbits[i] == j)
                prev[i] = j;
    }

    lastrejok = FALSE;
    for (i = 0; i < in; ++i)
        col[i] = 0;

    scan_stdvec(0, g, FALSE, prev, minedges, maxedges, 0, numcols, group, im, in, colpair);
}

static void
stringtograph(std::string s, graph *g, int m)
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

static int
scan_console(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges,
             long sofar, long numcols, grouprec *group, int m, int n, std::vector<int> &old_color)
/* Recursive scan for default case */
/* Returned value is level to return to. */
{
    int left;
    long min, max, k, ret;

    if (level == n)
    {
        //for (int i = 0; i < old_color.size(); i++)
        //{
        //    if (col[old_color[i]] == 0)
        //    {
        //        return level - 1;
        //    }
        //}




        return trythisone2console(group, g, digraph, m, n);
    }


    for (int i = 0; i < old_color.size(); i++)
    {
        if (col[old_color[i]] == 0)
        {
            col[old_color[i]] = 1;
            sofar++;
        }
    }


    left = n - level - 1;
    min = minedges - sofar - numcols * left;
    if (min < 0)
        min = 0;
    max = maxedges - sofar;
    if (max >= numcols)
        max = numcols - 1;
    if (prev[level] >= 0 && col[prev[level]] < max)
        max = col[prev[level]];






    for (k = min; k <= max; ++k)
    {


        for (int i = 0; i < old_color.size(); i++)
        {
            if (level == old_color[i])
            {
                level = old_color[i] + 1;
                continue;
            }
        }

        col[level] = k;
        ret = scan_console(level + 1, g, digraph, prev, minedges, maxedges, sofar + k, numcols, group, m, n, old_color);
        if (ret < level)
            return ret;
    }

    return level - 1;
}

static int
scan_stdvec(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges,
            long sofar, long numcols, grouprec *group, int m, int n, std::vector<std::vector<int>> &colpair)
/* Recursive scan for default case */
/* Returned value is level to return to. */
{
    int left;
    long min, max, k, ret;

    if (level == n)
        return trythisone2stdvec(group, g, digraph, m, n, colpair);

    left = n - level - 1;
    min = minedges - sofar - numcols * left;
    if (min < 0)
        min = 0;
    max = maxedges - sofar;
    if (max >= numcols)
        max = numcols - 1;
    if (prev[level] >= 0 && col[prev[level]] < max)
        max = col[prev[level]];

    for (k = min; k <= max; ++k)
    {
        col[level] = k;
        ret = scan_stdvec(level + 1, g, digraph, prev, minedges, maxedges, sofar + k, numcols, group, m, n, colpair);
        if (ret < level)
            return ret;
    }

    return level - 1;
}

static int
ismax(int *p, int n)
/* test if col^p <= col */
{
    int i, k;
    int fail;

    fail = 0;
    for (i = 0; i < n; ++i)
    {
        k = p[i];
        if (k > fail)
            fail = k;
        if (col[k] > col[i])
        {
            fail_level = fail;
            return FALSE;
        }
        else if (col[k] < col[i])
            return TRUE;
    }

    ++newgroupsize;
    return TRUE;
}

/**************************************************************************/

static void
testmax(int *p, int n, int *abort)
/* Called by allgroup2. */
{
    int i;

    if (first)
    { /* only the identity */
        first = FALSE;
        return;
    }

    if (!ismax(p, n))
    {
        *abort = 1;
        for (i = 0; i < n; ++i)
            lastreject[i] = p[i];
        lastrejok = TRUE;
    }
}

static int
trythisone2console(grouprec *group, graph *g, _Boolean digraph, int m, int n)
/* Try one solution, accept if maximal. */
/* Return value is level to return to. */
{
    int i, j;
    _Boolean accept;
    graph *gi;
    size_t ne;

    newgroupsize = 1;
    



    if (!group || groupsize == 1)
        accept = TRUE;
    else if (lastrejok && !ismax(lastreject, n))
        accept = FALSE;
    else if (lastrejok && groupsize == 2)
        accept = TRUE;
    else
    {
        newgroupsize = 1;
        first = TRUE;

        if (allgroup2(group, testmax) == 0)
            accept = TRUE;
        else
            accept = FALSE;
    }

    if (accept)
    {
        ++vc_nout;

        ne = 0;
        for (gi = g + m * (size_t)n; --gi >= g;)
            ne += POPCOUNT(*gi);
        if (!digraph)
        {
            for (i = 0, gi = g; i < n; ++i, gi += m)
                if (ISELEMENT(gi, i))
                    ++ne;
            ne /= 2;
        }
        // fprintf(outfile, "%d %lu", n, (unsigned long)ne);

        for (i = 0; i < n; ++i)
            if (col[i])
                std::cout << i << " ";
        std::cout << " ";
        // for (i = 0, gi = g; i < n; ++i, gi += m)
        // {
        //     for (j = (digraph ? -1 : i - 1); (j = nextelement(gi, m, j)) >= 0;)
        //         fprintf(outfile, " %d %d", i, j);
        // }
        std::cout << "\n";

        return n - 1;
    }
    else
        return fail_level - 1;
}

static int
trythisone2stdvec(grouprec *group, graph *g, _Boolean digraph, int m, int n, std::vector<std::vector<int>> &colpair)
/* Try one solution, accept if maximal. */
/* Return value is level to return to. */
{
    int i, j;
    _Boolean accept;
    graph *gi;
    size_t ne;
    std::vector<int> one_pattern;

    newgroupsize = 1;

    if (!group || groupsize == 1)
        accept = TRUE;
    else if (lastrejok && !ismax(lastreject, n))
        accept = FALSE;
    else if (lastrejok && groupsize == 2)
        accept = TRUE;
    else
    {
        newgroupsize = 1;
        first = TRUE;

        if (allgroup2(group, testmax) == 0)
            accept = TRUE;
        else
            accept = FALSE;
    }

    if (accept)
    {
        ++vc_nout;

        ne = 0;
        for (gi = g + m * (size_t)n; --gi >= g;)
            ne += POPCOUNT(*gi);
        if (!digraph)
        {
            for (i = 0, gi = g; i < n; ++i, gi += m)
                if (ISELEMENT(gi, i))
                    ++ne;
            ne /= 2;
        }
        // fprintf(outfile, "%d %lu", n, (unsigned long)ne);

        for (i = 0; i < n; ++i)
            if (col[i])
                one_pattern.push_back(i);
        colpair.push_back(one_pattern);
        // for (i = 0, gi = g; i < n; ++i, gi += m)
        // {
        //     for (j = (digraph ? -1 : i - 1); (j = nextelement(gi, m, j)) >= 0;)
        //         fprintf(outfile, " %d %d", i, j);
        // }
        one_pattern.clear();

        return n - 1;
    }
    else
        return fail_level - 1;
}

static int scan_fstream(int level, graph *g, _Boolean digraph, int *prev, long minedges, long maxedges, long sofar, long numcols, grouprec *group, int m, int n, std::ofstream &output_color_pattern_to_file)
{
    int left;
    long min, max, k, ret;

    if (level == n)
        return trythisone2fstream(group, g, digraph, m, n, output_color_pattern_to_file);

    left = n - level - 1;
    min = minedges - sofar - numcols * left;
    if (min < 0)
        min = 0;
    max = maxedges - sofar;
    if (max >= numcols)
        max = numcols - 1;
    if (prev[level] >= 0 && col[prev[level]] < max)
        max = col[prev[level]];

    for (k = min; k <= max; ++k)
    {
        col[level] = k;
        ret = scan_fstream(level + 1, g, digraph, prev, minedges, maxedges, sofar + k, numcols, group, m, n, output_color_pattern_to_file);
        if (ret < level)
            return ret;
    }

    return level - 1;
}
static int trythisone2fstream(grouprec *group, graph *g, _Boolean digraph, int m, int n, std::ofstream &output_color_pattern_to_file)
{
    int i, j;
    _Boolean accept;
    graph *gi;
    size_t ne;

    newgroupsize = 1;

    if (!group || groupsize == 1)
        accept = TRUE;
    else if (lastrejok && !ismax(lastreject, n))
        accept = FALSE;
    else if (lastrejok && groupsize == 2)
        accept = TRUE;
    else
    {
        newgroupsize = 1;
        first = TRUE;

        if (allgroup2(group, testmax) == 0)
            accept = TRUE;
        else
            accept = FALSE;
    }

    if (accept)
    {
        ++vc_nout;

        ne = 0;
        for (gi = g + m * (size_t)n; --gi >= g;)
            ne += POPCOUNT(*gi);
        if (!digraph)
        {
            for (i = 0, gi = g; i < n; ++i, gi += m)
                if (ISELEMENT(gi, i))
                    ++ne;
            ne /= 2;
        }
        // fprintf(outfile, "%d %lu", n, (unsigned long)ne);

        for (i = 0; i < n; ++i)
            if (col[i])
                output_color_pattern_to_file << std::setw(2) << i << " ";
        output_color_pattern_to_file << " ";
        // for (i = 0, gi = g; i < n; ++i, gi += m)
        // {
        //     for (j = (digraph ? -1 : i - 1); (j = nextelement(gi, m, j)) >= 0;)
        //         fprintf(outfile, " %d %d", i, j);
        // }
        output_color_pattern_to_file << "\n";

        return n - 1;
    }
    else
        return fail_level - 1;
}

static void
gt_abort(char *msg) /* Write message and halt. */
{
    if (msg)
        std::cerr << msg;
    exit(1);
}
