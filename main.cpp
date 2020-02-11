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


int main(int argc, const char **argv)
{
    double g = 0.1;
    double tolerance = 0.01;
    int d_size = 8;

    if (argc >= 4)
    {
        sscanf(argv[1], "%lf", &g);
        sscanf(argv[2], "%lf", &tolerance);
        sscanf(argv[3], "%i", &d_size);
    }

    MY_DEBUG_ONLY(cout << "g = " << g << endl;)
    MY_DEBUG_ONLY(cout << "tolerance = " << g << endl;)
    MY_DEBUG_ONLY(cout << "d_size = " << g << endl;)

    for(int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "d") == 0)
            IsDebugOutput = true;

    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
    #else
        //
    #endif

	MY_DEBUG_ONLY(cout << "Homework kkmeans (DEBUG detected)" << endl;)

    Task task;

    MY_DEBUG_ONLY(task.GenDemoData("DemoData.txt");)
    MY_DEBUG_ONLY(task.GenRealData("RealData.txt");)

    task.LoadDataFromInputStream();

    task.TrainAndDo(g, tolerance, d_size);

    return 0;
}

