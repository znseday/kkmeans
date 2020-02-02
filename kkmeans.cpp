#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>

#include "kkmeans.h"

using namespace std;


void TestFile(const char *file_name)
{
    cout << "File: " << file_name << endl;

    ifstream i_stream = ifstream(file_name);
    if (!i_stream)
    {
        cout << "My error: the input file not found" << endl;
        exit(0);
    }

    string line;
    while (getline(i_stream, line))
    {
        cout << line << endl; // just echo

    }    

    i_stream.close();
    cout << endl << endl;
}

