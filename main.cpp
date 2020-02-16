#include <iostream>
#include <fstream>
//#include <cstdio>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

#include "kkmeans.h"

using namespace std;

constexpr int MAX_CLUSTERS = 10000;

int main(int argc, const char **argv)
{
    int nClusters;

    double g = 0.1;
    double tolerance = 0.01;
    int d_size = 8;

    if (argc < 2)
    {
        cout << "There must be at least 1 parameter: count of clusters" << endl;
        exit(0);
    }
    sscanf(argv[1], "%i", &nClusters);

    if (nClusters < 1 || nClusters > MAX_CLUSTERS)
    {
        cout << "Error: something's wrong with count of clusters" << endl;
        exit(0);
    }

    if (argc >= 5)
    {
        sscanf(argv[2], "%lf", &g);
        sscanf(argv[3], "%lf", &tolerance);
        sscanf(argv[4], "%i", &d_size);
    }

    for(int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "d") == 0)
            IsDebugOutput = true;

    MY_DEBUG_ONLY(cout << "Homework kkmeans (DEBUG detected)" << endl;)

    MY_DEBUG_ONLY(cout << "nClusters = " << nClusters << endl;)
    MY_DEBUG_ONLY(cout << "g = " << g << endl;)
    MY_DEBUG_ONLY(cout << "tolerance = " << tolerance << endl;)
    MY_DEBUG_ONLY(cout << "d_size = " << d_size << endl;)

    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
    #else
        //
    #endif

    Task task;

    MY_DEBUG_ONLY(task.GenDemoData("DemoData.txt");)
    MY_DEBUG_ONLY(task.GenRealData("RealData.txt");)

    task.LoadDataFromInputStream();

    task.TrainAndDo(nClusters, g, tolerance, d_size);

    return 0;
}

