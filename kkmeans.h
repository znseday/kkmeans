#pragma once

#include <iostream>
#include <fstream>

#include <iterator>
#include <algorithm>
#include <memory>

#include <iostream>

//#include <dlib/clustering.h>

#include "C:/dlib-19.19/dlib-19.19/dlib/clustering.h"


// __FUNCSIG__ is for VS, but Qt (mingw) works with __PRETTY_FUNCTION__
#if ((defined WIN32) || (defined WIN64)) && (defined _MSC_VER)
#define MY_P_FUNC __FUNCSIG__
#else
#define MY_P_FUNC __PRETTY_FUNCTION__
#endif

extern bool IsDebugOutput;

#define MY_DEBUG_ONLY(x) { if(IsDebugOutput) {x}  }

//struct Point
//{
//    double x, y;
//};

class Task
{
private:
    std::vector<std::tuple<double,double>> data;

public:
    Task() = default;

    void GenDemoData(const std::string &fn);
    void GenRealData(const std::string &fn);

    void LoadDataFromInputStream();

    void TrainAndDo();
};



