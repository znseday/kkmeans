#include <iostream>
#include <fstream>
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

    task.TrainAndDo();

    return 0;
}

