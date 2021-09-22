//#include "cagesearch.h"
//
//#define USECMD
//// not define USECMD if you want to use cagesearch from file input and time output.
//// USECMD will make sure no output in cmd except error.
//
//int main(int argc, char *argv[])
//{
//    clock_t start, end;
//    start = clock();
//    // std::string inputstr = "{kKOK?B?Q??B?AG???W?AA????WG????SA?????g??@?C???C?@???B????O?G????_?C????o????O?@?????O?@?????o????@???_?????O?@?????B???A????????D???A?????????g???A??????@??C??????I????@???????C??_??????I?????C???????A??_??????@O?????A????????C?C??????C?G???????A_??????@?????????C@????????GG????????I?????????R\n";
//
//    /* UseCageSearch*/
//    // CageSearcher Cages;
//    // Cages.init_graph_from_str(inputstr);
//    // int Natoms = Cages.in;
//
//    // std::ofstream filetoout;
//    // filetoout.open("C60_1.out");
//
//    // std::vector<std::vector<int>> adjmat(Natoms, std::vector<int>(Natoms, 0));
//    // // Cages.output_graph(adjmat);
//    // std::vector<std::vector<int>> colnum;
//    // int addonnum = 8;
//    // Cages.output_color_pattern(addonnum, colnum);
//    // end = clock();
//    // double endtime = (double)(end - start) / CLOCKS_PER_SEC;
//    // std::cout << "Total time for " << addonnum << " addons is: " << endtime << " ms." << std::endl;
//    // std::cout << "Total variant for " << addonnum << " addons is: " << colnum.size() << " s." << std::endl;
//    // for (int i = 0; i < colnum.size(); i++)
//    // {
//    //     for (int j = 0; j < colnum[i].size(); j++)
//    //     {
//    //         filetoout << colnum[i][j] << " ";
//    //     }
//    //     filetoout << "\n";
//    // }
//    // filetoout.close();
//
//    // Use graph6 format:
//
//    // std::istrstream inputstrstream(inputstr.c_str());
//    // std::istream_iterator<unsigned char> inputgraph6(inputstrstream);
//    // graph6_to_edges_console<std::istream_iterator<unsigned char>>(inputgraph6);
//
//    cmdline::parser a;
//#ifdef USECMD
//    a.add<std::string>("graph6str", '\0', "graph6 format string", true);
//#else
//    a.add<std::string>("graph6file", '\0', "graph6 format string file", true);
//#endif
//    a.add<int>("addnum", 'n', "number of add atoms to the cage", true);
//    a.add<std::string>("outfile", 'o', "file to store output", false);
//    a.parse_check(argc, argv);
//    std::ostringstream outPutPathBuffer;
//
//#ifndef USECMD
//    std::string filein = a.get<std::string>("graph6file");
//    std::ifstream fin(filein);
//    std::string inputstr;
//    std::getline(fin, inputstr);
//#else
//    std::string inputstr = a.get<std::string>("graph6str");
//#endif
//
//    CageSearcher Cages;
//    Cages.init_graph_from_str(inputstr);
//    int Natoms = Cages.in;
//    int Naddons = a.get<int>("addnum");
//
//    if (!a.exist("outfile"))
//    {
//        outPutPathBuffer << "C" << Natoms << "_X" << Naddons;
//    }
//    else
//    {
//        outPutPathBuffer << a.get<std::string>("outfile");
//    }
//    std::string outPutPath = outPutPathBuffer.str();
//    std::ofstream outPutPathFileStream;
//    outPutPathFileStream.open(outPutPath);
//    Cages.output_color_pattern(Naddons, outPutPathFileStream);
//    end = clock();
//
//    double endtime = (double)(end - start) / CLOCKS_PER_SEC;
//#ifndef USECMD
//    std::cout << "Total time for " << Naddons << " addons is: " << endtime << " ms." << std::endl;
//#endif
//    return 0;
//}


#include "cagesearch.h"

#include<ctime>

int main()
{
    clock_t start, end;
    start = clock();
    std::string inputstr = "{kKS?L??h??DA?@?_@O?A?O???W?_???S?C????g?@???A@???I???@???_????B???O?????g???_?????S???A?????OG????D?????A????C??????@_????_??????D?????O???????g?????O??????GC??????I???????O?????A????????B??????C????????A_??????_????????D???????G????????OG???????@O???????G????????@@????????CG????????I?????????R\n";
    CageSearcher Cages;
    Cages.init_graph_from_str(inputstr);
    int Natoms = Cages.in;


    std::vector<std::vector<int>> adjmat(Natoms, std::vector<int>(Natoms, 0));
    // Cages.output_graph(adjmat);
    std::vector<std::vector<int>> colnum;
    int addonnum = 4;
    std::vector<int> old_color = {5, 11};


    Cages.output_color_pattern(addonnum, old_color);
    end = clock();
    double endtime = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "Total time for " << addonnum << " addons is: " << endtime << " ms";
    // for (int i = 0; i < colnum.size(); i++)
    // {
    //     for (int j = 0; j < colnum[i].size(); j++)
    //     {
    //         std::cout << colnum[i][j] << " Hi";
    //     }
    //     std::cout << std::endl;
    // }
}
